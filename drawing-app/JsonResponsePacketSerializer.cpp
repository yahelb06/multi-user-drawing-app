#include "JsonResponsePacketSerializer.h"

void JsonResponsePacketSerializer::getSizeIntoBuffer(int size, Buffer& buffer)
{
    buffer.push_back((size >> 24) & 0xFF);
    buffer.push_back((size >> 16) & 0xFF);
    buffer.push_back((size >> 8) & 0xFF);
    buffer.push_back(size & 0xFF);
}

Buffer JsonResponsePacketSerializer::serializeResponse(ErrResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::ERROR_CODE));
    nlohmann::json data;
    data["Message"] = res.message;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::LOGIN_REQUEST));
    nlohmann::json data;
    data["Status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(SignUpResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::SIGNUP_REQUEST));
    nlohmann::json data;
    data["Status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(RemoveUserResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::ERROR_CODE));
    nlohmann::json data;
    data["Status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(UserLogOutResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::USER_LOG_OUT_REQUEST));
    nlohmann::json data;
    data["Status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::CREATE_ROOM));
    nlohmann::json data;
    data["Status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::JOIN_ROOM));
    nlohmann::json data;
    data["Status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(RoomLogOutResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::ROOM_LOG_OUT_REQUEST));
    nlohmann::json data;
    data["Status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(AddUserResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::ADD_USER_TO_ROOM));
    nlohmann::json data;
    data["Status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(RemoveUserFromRoomResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::REMOVE_USER_FROM_ROOM));
    nlohmann::json data;
    data["Status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}
