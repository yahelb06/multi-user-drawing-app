#include "JsonResponsePacketSerializer.h"

void JsonResponsePacketSerializer::getSizeIntoBuffer(int size, Buffer& buffer)
{
    std::string lengthStr = std::to_string(size);

    while (lengthStr.length() < 6)
    {
        lengthStr = "0" + lengthStr;
    }
    buffer.insert(buffer.end(), lengthStr.begin(), lengthStr.end());
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
    data["RoomId"] = res.roomId;
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

Buffer JsonResponsePacketSerializer::serializeResponse(AcceptUserResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::ACCEPT_USER));
    nlohmann::json data;
    data["Status"] = res.status;
    data["RoomId"] = res.roomId;
    data["UsersInRoom"] = res.usersInRoom;
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

Buffer JsonResponsePacketSerializer::serializeResponse(RemovePaintFromRoomResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::REMOVE_PAINT_FROM_ROOM));
    nlohmann::json data;
    data["Status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(UploadPaintToRoomResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::UPLOAD_PAINT_TO_ROOM));
    nlohmann::json data;
    data["status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetUsersInRoomResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::GET_USERS_IN_ROOM));
    nlohmann::json data;
    data["usersInRoom"] = res.usersInRoom;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetUserPaintsNameResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::GET_USER_PAINTS));
    nlohmann::json data;
    data["paintsName"] = res.paintsName;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(AddLinesToPaintResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::REMOVE_PAINT_FROM_ROOM));
    nlohmann::json data;
    data["Status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetPaintFromRoomResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::GET_PAINT_FROM_ROOM));
    nlohmann::json data;
    data["PaintLines"] = getLinesVec(res.vecLines);
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetNewLinesResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::ADD_LINE_TO_PAINT));
    nlohmann::json data;
    data["PaintLines"] = getLinesVec(res.newLines);
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(SavePaintResponse& res)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::SAVE_PAINT));
    nlohmann::json data;
    data["status"] = res.status;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const std::string& userToAdd)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::JOIN_ROOM));
    nlohmann::json data;
    data["userToAdd"] = userToAdd;
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const Paint& paint)
{
    Buffer buffer;
    buffer.push_back(static_cast<unsigned char>(MessageCode::GET_PAINT_BY_NAME));
    nlohmann::json data;
    data["PaintLines"] = getLinesVec(paint.getPaintLines());
    data["paintName"] = paint.getPaintName();
    std::string jsonStr = data.dump();
    getSizeIntoBuffer(jsonStr.length(), buffer);
    buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
    return buffer;
}

nlohmann::json JsonResponsePacketSerializer::getLinesVec(const std::vector<Line>& vecLines)
{
    nlohmann::json linesArray = nlohmann::json::array();
    for (const auto& line : vecLines)
    {
        nlohmann::json jsonLine;
        jsonLine["start"]["x"] = line.getLine().first.coordinates.first;
        jsonLine["start"]["y"] = line.getLine().first.coordinates.second;

        jsonLine["end"]["x"] = line.getLine().second.coordinates.first;
        jsonLine["end"]["y"] = line.getLine().second.coordinates.second;
        jsonLine["color"] = line.getColor();

        linesArray.push_back(jsonLine);
    }
    return linesArray;
}
