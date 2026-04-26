#include "LoginRequestHandler.h"

bool LoginRequestHandler::isRequestRelevant(RequestInfo& info)
{
    return (info.id == char(MessageCode::LOGIN_REQUEST) || info.id == char(MessageCode::SIGNUP_REQUEST));
}

RequestResult LoginRequestHandler::handlerRequest(RequestInfo& info)
{
    RequestResult res;
    if (!isRequestRelevant(info))
    {
        ErrResponse err;
        err.message = "Request failed, Illegal message code.";
        res.response = JsonResponsePacketSerializer::serializeResponse(err);
    }
    else if (info.id == char(MessageCode::LOGIN_REQUEST))
    {
        res = login(info);
    }
    else if (info.id == char(MessageCode::SIGNUP_REQUEST))
    {

    }
    return res;
}

RequestResult LoginRequestHandler::login(RequestInfo& info)
{
    RequestResult res;

}
