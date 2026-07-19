using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Shapes;
using static DrawingApp.LoginWindow;

namespace DrawingApp
{

    public static class networkManager
    {
        public static TaskCompletionSource<byte[]> NetworkTaskSource;

        public static async Task<byte[]> SendAndReceiveAsync(byte[] packet)
        {
            NetworkTaskSource = new TaskCompletionSource<byte[]>();

            await LoginWindow.stream.WriteAsync(packet, 0, packet.Length);

            byte[] responseBuffer = await NetworkTaskSource.Task;

            return responseBuffer;
        }
        public static async Task SendOnlyAsync(byte[] packet)
        {
            await LoginWindow.stream.WriteAsync(packet, 0, packet.Length);
        }
    }

    internal class ServerMessages
    {

        public async Task ListenForServerMessages()
        {
            while (true)
            {
                try
                {
                    byte[] header = new byte[7];
                    int totalHeaderRead = 0;
                    while (totalHeaderRead < 7)
                    {
                        int read = await LoginWindow.stream.ReadAsync(header, totalHeaderRead, 7 - totalHeaderRead);
                        if (read == 0) return;
                        totalHeaderRead += read;
                    }

                    string lengthStr = Encoding.UTF8.GetString(header, 1, 6);
                    if (int.TryParse(lengthStr, out int jsonLength))
                    {
                        byte[] buffer = new byte[jsonLength];
                        int totalJsonRead = 0;
                        while (totalJsonRead < jsonLength)
                        {
                            int read = await LoginWindow.stream.ReadAsync(buffer, totalJsonRead, jsonLength - totalJsonRead);
                            if (read == 0) return;
                            totalJsonRead += read;
                        }
                        if (networkManager.NetworkTaskSource != null)
                        {
                            var taskSource = networkManager.NetworkTaskSource;
                            networkManager.NetworkTaskSource = null;
                            taskSource.SetResult(buffer);
                            continue;

                        }

                        byte messageCode = header[0];
                        byte[] fullPacket = new byte[7 + jsonLength];
                        Array.Copy(header, 0, fullPacket, 0, 7);
                        Array.Copy(buffer, 0, fullPacket, 7, jsonLength);
                        switch (messageCode)
                        {
                            case (byte)MessageCode.ERROR_CODE:
                                ShowErrorMsg(buffer);
                                break;
                            case (byte)LoginWindow.MessageCode.JOIN_ROOM:
                                JoinRoomMsg(buffer);
                                break;
                            case (byte)LoginWindow.MessageCode.ACCEPT_USER:
                                getAcceptFromManager(buffer);
                                break;
                            case (byte)LoginWindow.MessageCode.ADD_LINE_TO_PAINT:
                                addLine(buffer);
                                break;
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    break;
                }
            }
        }

        private void JoinRoomMsg(byte[] message)
        {
                Application.Current.Dispatcher.Invoke(async () =>
                {
                    try
                    {
                        using (JsonDocument doc = JsonDocument.Parse(message))
                        {
                            if (doc.RootElement.TryGetProperty("userToAdd", out JsonElement user))
                            {
                                var managerWindow = Application.Current.Windows.OfType<ManagerRoomWindow>().FirstOrDefault();
                                if (managerWindow != null)
                                {
                                    string usernameToAdd = user.GetString();

                                    MessageBoxResult result = MessageBox.Show(
                                        $"{usernameToAdd} wants to join your room",
                                        "Join request",
                                        MessageBoxButton.YesNo
                                    );

                                    if (result == MessageBoxResult.Yes)
                                    {
                                        await managerWindow.SendAddUser(true, usernameToAdd);
                                    }
                                    else
                                    {
                                        await managerWindow.SendAddUser(false, usernameToAdd);
                                    }
                                }
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error processing join request: " + ex.Message);
                    }
                });
        }
        private void getAcceptFromManager(byte[] message)
        {
                Application.Current.Dispatcher.Invoke(() =>
                {
                    try
                    {
                        using (JsonDocument doc = JsonDocument.Parse(message))
                        {
                            if (doc.RootElement.TryGetProperty("Status", out JsonElement status))
                            {
                                if (status.GetInt32() == 1)
                                {
                                    List<string> usersInRoom = new List<string>();
                                    string roomId = doc.RootElement.GetProperty("RoomId").GetString();
                                    if (doc.RootElement.TryGetProperty("UsersInRoom", out JsonElement usersArray))
                                    {
                                        foreach (JsonElement user in usersArray.EnumerateArray())
                                        {
                                            usersInRoom.Add(user.GetString());
                                        }

                                        RoomWindow roomWindow = new RoomWindow(roomId);
                                        roomWindow.Show();
                                        var oldWindow = Application.Current.Windows.OfType<JoinRoomWindow>().FirstOrDefault();
                                        if (oldWindow != null)
                                        {
                                            oldWindow.Close();
                                        }
                                    }
                                }
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error processing manager response: " + ex.Message);
                    }
                });
        }
        private async void addLine(byte[] message)
        {
            List<LineData> newLines = new List<LineData>();
            try
            {
                using (JsonDocument doc = JsonDocument.Parse(message))
                {
                    if (doc.RootElement.TryGetProperty("PaintLines", out JsonElement linesArray))
                    {
                        foreach (JsonElement line in linesArray.EnumerateArray())
                        {
                            uint x1 = line.GetProperty("start").GetProperty("x").GetUInt32();
                            uint y1 = line.GetProperty("start").GetProperty("y").GetUInt32();

                            uint x2 = line.GetProperty("end").GetProperty("x").GetUInt32();
                            uint y2 = line.GetProperty("end").GetProperty("y").GetUInt32();

                            string color = line.GetProperty("color").GetString();

                            LineData newLine = new LineData(new Coordinates(x1, y1), new Coordinates(x2, y2), color);

                            newLines.Add(newLine);
                        }
                    }
                }
                Application.Current.Dispatcher.Invoke(() =>
                {
                    var currWindow = Application.Current.Windows.OfType<RoomWindow>().FirstOrDefault();
                    if (currWindow != null)
                    {
                        currWindow.AddLinesToCanvas(newLines);
                    }
                });
            }
            catch (Exception ex)
            {
                MessageBox.Show("JSON Parse error in addLine: " + ex.Message);
            }
        }
    }
}
