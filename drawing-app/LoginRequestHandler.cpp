#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"

LoginRequestHandler::LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& handlerFactory) : m_loginManager(loginManager), m_handlerFactory(handlerFactory)
{
}

bool LoginRequestHandler::isRequestRelevant(RequestInfo& info)
{
    MessageCode code = static_cast<MessageCode>(info.id);
    return (code == MessageCode::LOGIN_REQUEST ||
        code == MessageCode::REMOVE_USER ||
        code == MessageCode::SIGNUP_REQUEST);
}

RequestResult LoginRequestHandler::handlerRequest(RequestInfo& info)
{
    RequestResult res;
    MessageCode code = static_cast<MessageCode>(info.id);
    if (!isRequestRelevant(info))
    {
        ErrResponse err;
        err.message = "Request failed, Illegal message code.";
        res.response = JsonResponsePacketSerializer::serializeResponse(err);
    }
    else if (code == MessageCode::LOGIN_REQUEST)
    {
        res = Login(info);
    }
    else if (code == MessageCode::SIGNUP_REQUEST)
    {
        res = SignUp(info);
    }
    else if (code == MessageCode::REMOVE_USER)
    {
        res = Remove(info);
    }
    return res;
}

RequestResult LoginRequestHandler::Login(RequestInfo& info)
{
    RequestResult res;

    LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);

    LoginStatus status = static_cast<LoginStatus>(this->m_loginManager.login(req.username, req.password));

    if (status == LoginStatus::LOGIN_SUCCESS)
    {
        LoginResponse login;
        login.status = static_cast<unsigned int>(status);
        res.response = JsonResponsePacketSerializer::serializeResponse(login);
        res.newHandler = nullptr;
        //res.newHandler = menu...
    }
    else if (status == LoginStatus::LOGIN_FAILED)
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

    SignUpRequest req = JsonRequestPacketDeserializer::deserializeSignUpRequest(info.buffer);
    
    SignUpStatus status;
    status = static_cast<SignUpStatus>(this->m_loginManager.signup(req.username, req.password, req.mail));
    if (status == SignUpStatus::SIGNUP_SUCCESS)
    {
        SignUpResponse signUp;
        signUp.status = static_cast<unsigned int>(status);
        res.response = JsonResponsePacketSerializer::serializeResponse(signUp);
        res.newHandler = nullptr;
        //res.newHandler = menu
    }
    else if (status == SignUpStatus::USER_ALREADY_EXISTS)
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

    RemoveUserRequest req = JsonRequestPacketDeserializer::deserializeRemoveUserRequest(info.buffer);
    
    RemoveStatus status;
    status = static_cast<RemoveStatus>(this->m_loginManager.Remove(req.username));

    if (status == RemoveStatus::REMOVE_SUCCESS)
    {
        RemoveUserResponse remove;
        remove.status = static_cast<unsigned int>(status);
        res.response = JsonResponsePacketSerializer::serializeResponse(remove);
        res.newHandler = this->m_handlerFactory.CreateLoginRequest();
    }
    else if (status == RemoveStatus::USER_NOT_FOUND)
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
