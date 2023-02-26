#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "my_service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using my::MyService;
using my::Request;
using my::Response;

class MyServiceClient {
 public:
  MyServiceClient(std::shared_ptr<Channel> channel)
      : stub_(MyService::NewStub(channel)) {}

  // 调用 SayHello 方法
  std::string SayHello(const std::string& name) {
    Request request;
    request.set_message(name);

    Response response;
    ClientContext context;

    Status status = stub_->SayHello(&context, request, &response);
    if (status.ok()) {
      return response.message();
    } else {
      std::cout << "Error: " << status.error_message() << std::endl;
      return "";
    }
  }

  // 调用 SayGoodbye 方法
  std::string SayGoodbye(const std::string& name) {
    Request request;
    request.set_message(name);

    Response response;
    ClientContext context;

    Status status = stub_->SayGoodbye(&context, request, &response);
    if (status.ok()) {
      return response.message();
    } else {
      std::cout << "Error: " << status.error_message() << std::endl;
      return "";
    }
  }

 private:
  std::unique_ptr<MyService::Stub> stub_;
};

int main(int argc, char** argv) {
  // 创建 MyServiceClient 客户端
  MyServiceClient client(
      grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

  // 调用 SayHello 方法
  std::string hello_response = client.SayHello("Alice");
  std::cout << "SayHello response: " << hello_response << std::endl;

  // 调用 SayGoodbye 方法
  std::string goodbye_response = client.SayGoodbye("Bob");
  std::cout << "SayGoodbye response: " << goodbye_response << std::endl;

  return 0;
}
