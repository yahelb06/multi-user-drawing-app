import socket
import json

LOGIN_CODE = 100
SIGNUP_CODE = 110

def getMsgData(need_mail):
    msgData = {
        "username" : input("enter username.\n"),
        "password" : input("enter password.\n")
    }
    if(need_mail):
        msgData["mail"] = input("enter your mail.\n")
    return msgData

def buildMsg(useropt, msgData_dict):
    msgCodes = {"2": chr(100), "3": chr(110)}
    msg_code = msgCodes[useropt]
    json_str = json.dumps(msgData_dict)

    length = str(len(json_str)).zfill(4)
    final_str = msg_code + length + json_str
      
    return final_str

def main():
    host = "127.0.0.1"
    port = 8826 

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        client_socket.connect((host, port))
        
        while True:
            actions = {
                "2": {"opcode" : LOGIN_CODE, "needs_mail" : False},
                "3": {"opcode" : SIGNUP_CODE, "needs_mail" : True}
            }
            useropt = input("Enter option:\n2 - Login\n3 - Signup\n")
            if useropt in actions:
                config = actions[useropt]
                msgData = getMsgData(config["needs_mail"])
                msg = buildMsg(useropt, msgData)

                client_socket.sendall(msg.encode())
                newMsg = client_socket.recv(1024).decode()
                print(newMsg)

    except ConnectionRefusedError:
        print("Error: Could not connect.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
    finally:
        client_socket.close()

if __name__ == '__main__':
    main()
