#define DECOMPRESSION_BUF_SIZE 100000

#include <queue>
#include <fstream>
#include <lz4.h>
#include <lz4frame.h>

#include <flatbuffers/flatbuffers.h>
#include "flatbuffers/ioheader_generated.h"
#include "flatbuffers/file_data_table_generated.h"
#include "flatbuffers/events_generated.h"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <opencv2/core.hpp>

namespace py = pybind11;

class Aedat{
    public:
        Aedat(std::string filename);
        // const EventPacket * read_packet(int &event_cnt);
        py::array_t<uint64_t> read(py::array_t<uint8_t>& frame);
    private:
        LZ4F_decompressionContext_t lz4_ctx;
        uint8_t *data_cur;
        uint8_t *data_start;
        uint8_t *data_end;
};