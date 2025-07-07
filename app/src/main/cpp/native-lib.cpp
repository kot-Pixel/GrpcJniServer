#include <jni.h>
#include <string>
#include <sys/stat.h>
#include "CarplayUnixDomainSocketServer.h"



void RunUDSServer() {
    // 1. 初始化 gRPC 环境
    grpc_init();

    // 2. 指定 Unix Domain Socket 路径
    std::string uds_address("unix:///data/local/tmp/grpc.sock");

    // 3. 删除已存在的 socket 文件（如果存在）
    unlink("/data/local/tmp/grpc.sock");

    GreeterServiceImpl service;

    ServerBuilder builder;
    // 4. 监听 Unix Domain Socket
    builder.AddListeningPort(uds_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    // 5. 构建并启动服务器
    std::unique_ptr<Server> server(builder.BuildAndStart());
    if (!server) {
        LOGE("Failed to start server");

        // 获取系统级错误信息
        LOGE("System error: %s", strerror(errno));

        grpc_shutdown();
        return;
    }

    chmod("/data/local/tmp/grpc.sock", 0660);
    LOGI("Server listening on Unix Domain Socket:  %s", uds_address.data());

    // 6. 等待服务器关闭
    server->Wait();

    // 7. 清理 gRPC 环境
    grpc_shutdown();

    LOGI("grpc server shut down");
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_kotlinx_grpcjniserver_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    RunUDSServer();

    return env->NewStringUTF(hello.c_str());
}