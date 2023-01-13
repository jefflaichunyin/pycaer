#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>

#include <libcaercpp/devices/davis.hpp>
#include <libcaercpp/filters/dvs_noise.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;

using namespace std;

libcaer::devices::davis davisHandle = libcaer::devices::davis(1);
struct caer_davis_info davis_info = davisHandle.infoGet();
libcaer::filters::DVSNoise dvsNoiseFilter = libcaer::filters::DVSNoise(davis_info.dvsSizeX, davis_info.dvsSizeX);
// cv::Mat cvEvents(davis_info.dvsSizeY, davis_info.dvsSizeX, CV_8UC3, cv::Vec3b{127, 127, 127});

static void usbShutdownHandler(void *ptr) {
	(void) (ptr); // UNUSED.
    perror("USB Disconnected");
    davisHandle.dataStop();
    exit(EXIT_SUCCESS);
}

int start() {
	printf("%s --- ID: %d, Master: %d, DVS X: %d, DVS Y: %d, Logic: %d.\n", davis_info.deviceString,
		davis_info.deviceID, davis_info.deviceIsMaster, davis_info.dvsSizeX, davis_info.dvsSizeY,
		davis_info.logicVersion);
	// Send the default configuration before using the device.
	// No configuration is sent automatically!
	davisHandle.sendDefaultConfig();

	// Enable hardware filters if present.
	if (davis_info.dvsHasBackgroundActivityFilter) {
		davisHandle.configSet(DAVIS_CONFIG_DVS, DAVIS_CONFIG_DVS_FILTER_BACKGROUND_ACTIVITY_TIME, 8);
		davisHandle.configSet(DAVIS_CONFIG_DVS, DAVIS_CONFIG_DVS_FILTER_BACKGROUND_ACTIVITY, true);

		davisHandle.configSet(DAVIS_CONFIG_DVS, DAVIS_CONFIG_DVS_FILTER_REFRACTORY_PERIOD_TIME, 1);
		davisHandle.configSet(DAVIS_CONFIG_DVS, DAVIS_CONFIG_DVS_FILTER_REFRACTORY_PERIOD, true);
	}

	// Add full-sized software filter to reduce DVS noise.
	libcaer::filters::DVSNoise dvsNoiseFilter = libcaer::filters::DVSNoise(davis_info.dvsSizeX, davis_info.dvsSizeX);

	dvsNoiseFilter.configSet(CAER_FILTER_DVS_BACKGROUND_ACTIVITY_TWO_LEVELS, true);
	dvsNoiseFilter.configSet(CAER_FILTER_DVS_BACKGROUND_ACTIVITY_CHECK_POLARITY, true);
	dvsNoiseFilter.configSet(CAER_FILTER_DVS_BACKGROUND_ACTIVITY_SUPPORT_MIN, 2);
	dvsNoiseFilter.configSet(CAER_FILTER_DVS_BACKGROUND_ACTIVITY_SUPPORT_MAX, 8);
	dvsNoiseFilter.configSet(CAER_FILTER_DVS_BACKGROUND_ACTIVITY_TIME, 2000);
	dvsNoiseFilter.configSet(CAER_FILTER_DVS_BACKGROUND_ACTIVITY_ENABLE, true);

	dvsNoiseFilter.configSet(CAER_FILTER_DVS_REFRACTORY_PERIOD_TIME, 200);
	dvsNoiseFilter.configSet(CAER_FILTER_DVS_REFRACTORY_PERIOD_ENABLE, true);

	dvsNoiseFilter.configSet(CAER_FILTER_DVS_HOTPIXEL_ENABLE, true);
	dvsNoiseFilter.configSet(CAER_FILTER_DVS_HOTPIXEL_LEARN, true);

	// Now let's get start getting some data from the device. We just loop in blocking mode,
	// no notification needed regarding new events. The shutdown notification, for example if
	// the device is disconnected, should be listened to.
	davisHandle.dataStart(nullptr, nullptr, nullptr, &usbShutdownHandler, nullptr);

	// Let's turn on blocking data-get mode to avoid wasting resources.
	davisHandle.configSet(CAER_HOST_CONFIG_DATAEXCHANGE, CAER_HOST_CONFIG_DATAEXCHANGE_BLOCKING, true);

	// Disable APS (frames) and IMU, not used for showing event filtering.
	davisHandle.configSet(DAVIS_CONFIG_APS, DAVIS_CONFIG_APS_RUN, false);
	davisHandle.configSet(DAVIS_CONFIG_IMU, DAVIS_CONFIG_IMU_RUN_ACCELEROMETER, false);
	davisHandle.configSet(DAVIS_CONFIG_IMU, DAVIS_CONFIG_IMU_RUN_GYROSCOPE, false);
	davisHandle.configSet(DAVIS_CONFIG_IMU, DAVIS_CONFIG_IMU_RUN_TEMPERATURE, false);        
    return EXIT_SUCCESS;
}

int stop() {
    davisHandle.dataStop();
    return EXIT_SUCCESS;
}

py::array_t<uint64_t> get_packet(py::array_t<uint8_t>& frame) {
    int event_cnt = 0;
    auto empty = py::array(py::buffer_info(
        NULL,
        sizeof(uint64_t),
        py::format_descriptor<uint64_t>::value,
        2,
        {0, 0},
        {sizeof(uint64_t)*4, sizeof(uint64_t)}
    ));
    std::unique_ptr<libcaer::events::EventPacketContainer> packetContainer = davisHandle.dataGet();
    
    if (packetContainer == nullptr) {
        return empty;
    }

    for (auto &packet : *packetContainer) {
        if (packet == nullptr) {
            continue;
        }
        if (packet->getEventType() != POLARITY_EVENT) {
            continue;
        } else {
            std::shared_ptr<libcaer::events::PolarityEventPacket> polarity
                = std::static_pointer_cast<libcaer::events::PolarityEventPacket>(packet);

            dvsNoiseFilter.apply(*polarity);
            event_cnt = polarity->getEventValid();
            // printf("Got polarity packet with %d events, after filtering remaining %d events.\n",
            //     polarity->getEventNumber(), polarity->getEventValid());

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
            cv::Mat cvEvents(frame.shape(0), frame.shape(1), CV_8UC3, (unsigned char*)frame.data());
            int i = 0;
            for (const auto &e : *polarity) {
                // Discard invalid events (filtered out).
                if (!e.isValid()) {
                    continue;
                }
                events_buf[4*i + 0] = e.getX();
                events_buf[4*i + 1] = e.getY();
                events_buf[4*i + 2] = e.getTimestamp();
                events_buf[4*i + 3] = e.getPolarity();
                i++;
                cvEvents.at<cv::Vec3b>(e.getY(), e.getX())
                    = e.getPolarity() ? cv::Vec3b{0, 0, 255} : cv::Vec3b{255, 0, 0};
            }
            return events;
        }
        return empty;
    }
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

    m.def("read", &get_packet);
    
#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
