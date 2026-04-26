#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"

LoginRequestHandler::LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& handlerFactory) : m_loginManager(loginManager), m_handlerFactory(handlerFactory)
{
}

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
        res = Login(info);
    }
    else if (info.id == char(MessageCode::SIGNUP_REQUEST))
    {
        res = SignUp(info);
    }
    return res;
}

RequestResult LoginRequestHandler::Login(RequestInfo& info)
{
    RequestResult res;

    Buffer buffer;
    buffer.insert(buffer.begin(), info.buffer.begin(), info.buffer.end());
    LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(buffer);
    
    LoginResponse login;
    login.status = unsigned int(this->m_loginManager.login(req.username, req.password));

    if (login.status == unsigned int(LoginStatus::LOGIN_SUCCESS))
    {
        res.response = JsonResponsePacketSerializer::serializeResponse(login);
        res.newHandler = nullptr;
        //res.newHandler = menu...
    }
    else if(login.status == unsigned int(LoginStatus::LOGIN_FAILED))
    {
        ErrResponse err;
        err.message = "Login failed. Please try again.";
        res.response = JsonResponsePacketSerializer::serializeResponse(err);
        res.newHandler = m_handlerFactory.CreateLoginRequest();
    }
    else
    {
        ErrResponse err;
        err.message = "User is already logged in. Please try again later.";
        res.response = JsonResponsePacketSerializer::serializeResponse(err);
        res.newHandler = m_handlerFactory.CreateLoginRequest();
    }
    return res;
}

RequestResult LoginRequestHandler::SignUp(RequestInfo& info)
{
    RequestResult res;

    Buffer buffer;
    buffer.insert(buffer.begin(), info.buffer.begin(), info.buffer.end());

    SignUpRequest req = JsonRequestPacketDeserializer::deserializeSignUpRequest(buffer);
    SignUpResponse signUp;
    signUp.status = unsigned int(this->m_loginManager.signup(req.username, req.password, req.mail));
    if (signUp.status == unsigned int(SignUpStatus::SIGNUP_SUCCESS))
    {
        res.response = JsonResponsePacketSerializer::serializeResponse(signUp);
        res.newHandler = nullptr;
        //res.newHandler = menu
    }
    else if (signUp.status == unsigned int(SignUpStatus::USER_ALREADY_EXISTS))
    {
        ErrResponse err;
        err.message = "There is already user with this name. Please try other name.";
        res.response = JsonResponsePacketSerializer::serializeResponse(err);
        res.newHandler = m_handlerFactory.CreateLoginRequest();
    }
    else
    {
        ErrResponse err;
        err.message = "Sign up failed. Please try again.";
        res.response = JsonResponsePacketSerializer::serializeResponse(err);
        res.newHandler = m_handlerFactory.CreateLoginRequest();
    }
    return res;
}

RequestResult LoginRequestHandler::Remove(RequestInfo& info)
{
    RequestResult res;

    Buffer buffer;
    buffer.insert(buffer.end(), info.buffer.begin(), info.buffer.begin());

    RemoveUserRequest req = JsonRequestPacketDeserializer::deserializeRemoveUserRequest(buffer);
    RemoveUserResponse remove;
    remove.status = unsigned int(this->m_loginManager.Remove(req.username));

    if (remove.status == unsigned int(RemoveStatus::REMOVE_SUCCESS))
    {
        res.response = JsonResponsePacketSerializer::serializeResponse(remove);
        res.newHandler = this->m_handlerFactory.CreateLoginRequest();
    }
    else if (remove.status == unsigned int(RemoveStatus::USER_NOT_FOUND))
    {
        ErrResponse err;
        err.message = "User not found.";
        res.response = JsonResponsePacketSerializer::serializeResponse(err);
        res.newHandler = this->m_handlerFactory.CreateLoginRequest();
    }
    else
    {
        ErrResponse err;
        err.message = "Remove failed. Please try again.";
        res.response = JsonResponsePacketSerializer::serializeResponse(err);
        res.newHandler = this->m_handlerFactory.CreateLoginRequest();
    }
    return res;
}
