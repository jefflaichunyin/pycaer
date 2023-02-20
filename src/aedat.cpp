#include <iostream>

#include "aedat.hpp"

// modified from https://github.com/norse/aestream/blob/main/src/aedat4.hpp

#include <stdio.h>
#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

Aedat::Aedat(std::string filename)
{
    timestamp_valid = false;
    std::ifstream infile;
    infile.open(filename, std::ios::binary | std::ios::in);

    if (infile.fail())
    {
        std::cout << "Failed to open the input file" << std::endl;
        throw std::runtime_error("Failed to open the input file");
    }
    std::cout << "File opened\n";

    infile.seekg(0, std::ios::end);
    int file_size = infile.tellg();
    infile.seekg(0, std::ios::beg);
    char *data = new char[file_size];
    char *buffer_start = data;

    infile.read(data, file_size); // TODO: use mmap
    infile.close();

    std::cout << "Read " << file_size << " bytes\n";

    auto header = std::string(data, 14);
    std::cout << "Header: " << header << std::endl;
    if (header != "#!AER-DAT4.0\r\n")
    {
        throw std::runtime_error("Invalid AEDAT version");
    }

    data += 14; // size of the version string
    flatbuffers::uoffset_t ioheader_offset =
        *reinterpret_cast<flatbuffers::uoffset_t *>(data);
    std::cout << "IO header offset = " << ioheader_offset << std::endl;

    const IOHeader *ioheader = GetSizePrefixedIOHeader(data);
    int64_t data_table_position = ioheader->data_table_position();
    std::cout << "Data table position = " << data_table_position << std::endl;
    if (data_table_position < 0)
    {
        throw std::runtime_error(
            "AEDAT files without datatables are currently not supported");
    }
    char *data_table_start = buffer_start + data_table_position;

    data += ioheader_offset + 4;
    // start of data stream
    LZ4F_errorCode_t lz4_error =
        LZ4F_createDecompressionContext(&lz4_ctx, LZ4F_VERSION);

    if (LZ4F_isError(lz4_error))
    {
        throw std::runtime_error("Decompression context error");
    }
    
    data_cur = (uint8_t*)data;
    data_start = (uint8_t*)data;
    data_end = (uint8_t*)data_table_start;
}

py::array_t<uint64_t> Aedat::read(std::optional<py::array_t<uint8_t>>& frame){
    static uint8_t dst_buf[DECOMPRESSION_BUF_SIZE];
    int32_t stream_id;
    size_t pkt_size;

    int event_cnt = 0;
    auto empty = py::array(py::buffer_info(
        NULL,
        sizeof(uint64_t),
        py::format_descriptor<uint64_t>::value,
        2,
        {0, 0},
        {sizeof(uint64_t)*4, sizeof(uint64_t)}
    ));
    empty.request();
    stream_id = *reinterpret_cast<int32_t *>(data_cur);
    data_cur += 4;
    pkt_size = *reinterpret_cast<int32_t *>(data_cur);
    data_cur += 4;

    while(stream_id != 0 && data_cur < data_end){
        data_cur += pkt_size;
        stream_id = *reinterpret_cast<int32_t *>(data_cur);
        data_cur += 4;
        pkt_size = *reinterpret_cast<int32_t *>(data_cur);
        data_cur += 4;
    }
    
    // std::cout << "stream id: " << stream_id << " size: " << pkt_size << std::endl;

    if(stream_id != 0 || pkt_size == 0 || data_cur >= data_end)
    {
        event_cnt = 0;
        return empty;
    }

    // event data
    size_t read_len = pkt_size;
    size_t write_len = sizeof(dst_buf);
    size_t remain = LZ4F_decompress(lz4_ctx, dst_buf, &write_len, data_cur, &read_len, nullptr);
    data_cur += pkt_size;
    
    if (remain > 0)
    {
        std::cout << "pkt size: " << pkt_size << "read: " << read_len << " write: " << write_len << std::endl;
        throw std::runtime_error("Incomplete decompression, increase buffer size");
        event_cnt = 0;
        return empty;
    }

    auto event_pkt = GetSizePrefixedEventPacket(dst_buf);
    event_cnt = event_pkt->elements()->Length();
    auto events = py::array(py::buffer_info(
        nullptr,
        sizeof(uint64_t),
        py::format_descriptor<uint64_t>::value,
        2,
        {event_cnt, 4},
        {sizeof(uint64_t)*4, sizeof(uint64_t)}
    ));
    py::buffer_info buf_inf = events.request(true);
    auto events_buf = (uint64_t *)buf_inf.ptr;

    int i = 0;
    if(!timestamp_valid) {
        timestamp_offset = event_pkt->elements()->Get(0)->t();
        timestamp_valid = true;
    }
    if(!frame.has_value()) {
        for(const auto &e : *event_pkt->elements()) {
            events_buf[4*i + 0] = e->x();
            events_buf[4*i + 1] = e->y();
            events_buf[4*i + 2] = e->t() - timestamp_offset;
            events_buf[4*i + 3] = e->on();
            i++;
        }
    } else {
        cv::Mat cvEvents(frame.value().shape(0), frame.value().shape(1), CV_8UC3, (unsigned char*)frame.value().data());
        for(const auto &e : *event_pkt->elements()) {
            events_buf[4*i + 0] = e->x();
            events_buf[4*i + 1] = e->y();
            events_buf[4*i + 2] = e->t() - timestamp_offset;
            events_buf[4*i + 3] = e->on();
            i++;
            cvEvents.at<cv::Vec3b>(e->y(), e->x())
                = e->on() ? cv::Vec3b{0, 0, 255} : cv::Vec3b{0, 255, 0};
        }
    }
    return events;
}

PYBIND11_MODULE(aedat, m) {
    py::class_<Aedat>(m, "Aedat")
        .def(py::init<std::string>())
        .def("read", &Aedat::read, py::arg("frame") = py::none());
}