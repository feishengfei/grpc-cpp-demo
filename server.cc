#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "my_service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using my::MyService;
using my::Request;
using my::Response;

// 实现 MyService 接口
class MyServiceImpl final : public MyService::Service {
 public:
  Status SayHello(ServerContext* context, const Request* request,
                  Response* response) override {
    std::string prefix("Hello ");
    response->set_message(prefix + request->message());
    return Status::OK;
  }

  Status SayGoodbye(ServerContext* context, const Request* request,
                    Response* response) override {
    std::string prefix("Goodbye ");
    response->set_message(prefix + request->message());
    return Status::OK;
  }
};

// 启动 gRPC 服务
void RunServer() {
  std::string server_address("0.0.0.0:50051");
  MyServiceImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}
