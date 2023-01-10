#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>
#include <libcaer/libcaer.h>
#include <libcaer/devices/davis.h>

#include <stdio.h>
#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;

caerDeviceHandle davis_handle;

int start() {
    davis_handle = caerDeviceOpen(1, CAER_DEVICE_DAVIS, 0, 0, NULL);
    if(davis_handle == NULL) {
        perror("Cannot open device");
        return (EXIT_FAILURE);
    }

    caerDeviceSendDefaultConfig(davis_handle);
    
	struct caer_bias_coarsefine coarseFineBias;

	coarseFineBias.coarseValue        = 2;
	coarseFineBias.fineValue          = 116;
	coarseFineBias.enabled            = true;
	coarseFineBias.sexN               = false;
	coarseFineBias.typeNormal         = true;
	coarseFineBias.currentLevelNormal = true;
	caerDeviceConfigSet(davis_handle, DAVIS_CONFIG_BIAS, DAVIS240_CONFIG_BIAS_PRBP, caerBiasCoarseFineGenerate(coarseFineBias));

	coarseFineBias.coarseValue        = 1;
	coarseFineBias.fineValue          = 33;
	coarseFineBias.enabled            = true;
	coarseFineBias.sexN               = false;
	coarseFineBias.typeNormal         = true;
	coarseFineBias.currentLevelNormal = true;
	caerDeviceConfigSet(davis_handle, DAVIS_CONFIG_BIAS, DAVIS240_CONFIG_BIAS_PRSFBP, caerBiasCoarseFineGenerate(coarseFineBias));
	
    caerDeviceDataStart(davis_handle, NULL, NULL, NULL, NULL, NULL);
    caerDeviceConfigSet(davis_handle, CAER_HOST_CONFIG_DATAEXCHANGE, CAER_HOST_CONFIG_DATAEXCHANGE_BLOCKING, true);

    return EXIT_SUCCESS;
}

int stop() {
    if(davis_handle == NULL) {
        perror("Device not yet opened");
        return (EXIT_FAILURE);
    }
    caerDeviceDataStop(davis_handle);
    caerDeviceClose(&davis_handle);
    davis_handle = NULL;
    return EXIT_SUCCESS;
}

int get_info() {
    if(davis_handle == NULL) {
        perror("Device not yet opened");
        return EXIT_FAILURE;
    }
	struct caer_davis_info davis_info = caerDavisInfoGet(davis_handle);

	printf("%s --- ID: %d, Master: %d, DVS X: %d, DVS Y: %d, Logic: %d.\n", davis_info.deviceString,
		davis_info.deviceID, davis_info.deviceIsMaster, davis_info.dvsSizeX, davis_info.dvsSizeY,
		davis_info.logicVersion);
    return EXIT_SUCCESS;
}

py::array_t<uint64_t> get_packet() {
    int event_cnt = 0;
    auto empty = py::array(py::buffer_info(
        NULL,
        sizeof(uint64_t),
        py::format_descriptor<uint64_t>::value,
        2,
        {0, 0},
        {sizeof(uint64_t)*4, sizeof(uint64_t)}
    ));

    if(davis_handle == NULL) {
        perror("Device not yet opened, open it now");
        if(start() == EXIT_FAILURE)
            return empty;
    }

    caerEventPacketContainer packetContainer = caerDeviceDataGet(davis_handle);
    if (packetContainer == NULL) {
        return empty;
    }
    int32_t packetNum = caerEventPacketContainerGetEventPacketsNumber(packetContainer);
    if (packetNum < 2) {
        caerEventPacketContainerFree(packetContainer);
        return empty;
    }
    // only get polarity event and ignore other events
    caerEventPacketHeader packetHeader = caerEventPacketContainerGetEventPacket(packetContainer, POLARITY_EVENT);
    if (packetHeader == NULL) {
        perror("packet header is NULL");
        return empty;
    }
    event_cnt = caerEventPacketHeaderGetEventNumber(packetHeader);
    caerPolarityEventPacket polarity = (caerPolarityEventPacket) packetHeader;

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
    for(int i = 0; i<event_cnt; i++) {
        caerPolarityEventConst event = caerPolarityEventPacketGetEventConst(polarity, i);
        int32_t ts = caerPolarityEventGetTimestamp(event);
        uint16_t x = caerPolarityEventGetX(event);
        uint16_t y = caerPolarityEventGetY(event);
        bool pol   = caerPolarityEventGetPolarity(event);
        events_buf[4*i + 0] = x;
        events_buf[4*i + 1] = y;
        events_buf[4*i + 2] = ts;
        events_buf[4*i + 3] = pol;
    }
    caerEventPacketContainerFree(packetContainer);
    return events;
}

PYBIND11_MODULE(davis, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: python_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    m.def("open", &start, R"pbdoc(
        Open the connection to Davis camera
    )pbdoc");

    m.def("close", &stop, R"pbdoc(
        Close the connection to Davis camera
    )pbdoc");



    m.def("info", &get_info, R"pbdoc(
        Read a info from Davis camera
    )pbdoc");

    m.def("read", &get_packet);
    
#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
