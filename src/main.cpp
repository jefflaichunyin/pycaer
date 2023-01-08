#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <libcaer/libcaer.h>
#include <libcaer/devices/davis.h>

#include <stdio.h>
#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

caerDeviceHandle davis_handle;

int get_packet();

int start() {
    davis_handle = caerDeviceOpen(1, CAER_DEVICE_DAVIS, 0, 0, NULL);
    if(davis_handle == NULL) {
        perror("Cannot open device");
        return (EXIT_FAILURE);
    }

    caerDeviceSendDefaultConfig(davis_handle);
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

int get_packet() {
    int event_cnt = 0;
    if(davis_handle == NULL) {
        perror("Device not yet opened, open it now");
        if(start() == EXIT_FAILURE)
            return 0;
    }

    caerEventPacketContainer packetContainer = caerDeviceDataGet(davis_handle);
    if (packetContainer == NULL) {
        return 0;
    }
    int32_t packetNum = caerEventPacketContainerGetEventPacketsNumber(packetContainer);
    if (packetNum < 1) {
        return 0;
    }
    // only get polarity event and ignore other events
    caerEventPacketHeader packetHeader = caerEventPacketContainerGetEventPacket(packetContainer, POLARITY_EVENT);
    event_cnt = caerEventPacketHeaderGetEventNumber(packetHeader);
    caerPolarityEventPacket polarity = (caerPolarityEventPacket) packetHeader;
    // Get full timestamp and addresses of first event.
    caerPolarityEventConst firstEvent = caerPolarityEventPacketGetEventConst(polarity, 0);

    int32_t ts = caerPolarityEventGetTimestamp(firstEvent);
    uint16_t x = caerPolarityEventGetX(firstEvent);
    uint16_t y = caerPolarityEventGetY(firstEvent);
    bool pol   = caerPolarityEventGetPolarity(firstEvent);

    printf("First polarity event - ts: %d, x: %d, y: %d, pol: %d.\n", ts, x, y, pol);

    caerEventPacketContainerFree(packetContainer);

    return event_cnt;
}

namespace py = pybind11;

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

    m.def("read", &get_packet, R"pbdoc(
        Read a packet from Davis camera
    )pbdoc");

    m.def("info", &get_info, R"pbdoc(
        Read a info from Davis camera
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
