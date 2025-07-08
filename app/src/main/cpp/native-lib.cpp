#include <jni.h>
#include <string>
#include <sys/stat.h>
#include <thread>
#include "CarplayBtServiceImpl.h"

void RunUDSServer() {
    grpc_init();
    const std::string uds_address = "unix-abstract:carplay_bt";
    CarplayBtServiceImpl service;
    ServerBuilder builder;
    builder.AddListeningPort(uds_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    if (!server) {
        LOGE("Failed to start server, error: %s", strerror(errno));
        grpc_shutdown();
        return;
    }

    LOGI("Server listening on Unix Domain Socket:  %s", uds_address.data());
    server->Wait();
    grpc_shutdown();
    LOGI("grpc server shut down");
}


extern "C"
JNIEXPORT void JNICALL
Java_com_kotlinx_grpcjniserver_MainActivity_runUdpCarplayServer(JNIEnv *env, jobject thiz) {
    std::thread grpc_thread(RunUDSServer);
    grpc_thread.detach();
}