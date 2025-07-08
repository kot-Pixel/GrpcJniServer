//
// Created by Tom on 2025/7/8.
//
#include "rfcomm.protoc.grpc.pb.h"

#include "CarplayNativeLogger.h"
#include "grpcpp/security/server_credentials.h"
#include "grpcpp/server_builder.h"

using grpc::ServerBuilder;
using grpc::Server;

class CarplayBtServiceImpl final : public carplay::bt::CarplayBtService::Service::Service {
public:
    grpc::Status RfcommTransport(::grpc::ServerContext *context,
                                 ::grpc::ServerReaderWriter<::carplay::bt::RfcommPacket, ::carplay::bt::RfcommPacket> *stream) override {

        carplay::bt::RfcommPacket packet;

        while (stream->Read(&packet)) {
            auto data = packet.payload();
            auto dir = packet.direction();

            LOGI("[Server] Received %s", data.data());

            carplay::bt::RfcommPacket reply;
            reply.set_direction(carplay::bt::RfcommTransportDirection::OUT);
            reply.set_payload(data);

            stream->Write(reply);
        }

        return grpc::Status::OK;
    }
};
