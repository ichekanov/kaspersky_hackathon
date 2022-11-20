import paho.mqtt.publish as publish
import json
import cv2
import numpy as np

test_json = {
    "cmd": "auto",
    "markers_screen": [{"id": "0", "x": "102.0", "y": "603.0"}, {"id": "2", "x": "375.0", "y": "347.0"},
                       {"id": "3", "x": "860.0", "y": "342.0"}, {"id": "4", "x": "1038.0", "y": "610.0"}],
    "markers_floor": [{"id": "0", "x": "1.0", "y": "4.0"}, {"id": "2", "x": "1.0", "y": "1.0"},
                      {"id": "3", "x": "4.0", "y": "1.0"}, {"id": "4", "x": "4.0", "y": "4.0"}],
    "targets_screen": [{"id": "2", "x": "470.0", "y": "475.0"},
                       {"id": "3", "x": "703.0", "y": "400.0"}, {"id": "4", "x": "706.0", "y": "480.0"},
                       {"id": "5", "x": "717.0", "y": "606.0"}],
    "robot_screen": [{"id": "0", "x": "511.0", "y": "400.0"}]
}


def control():
    print("1 - left")
    print("2 - back")
    print("3 - right")
    print("5 - forward")
    print("0 - stop")

    while (True):
        cmd = list(map(float,input().split()))
        if cmd[0] == 0:
            publish.single("abot/command", "{\"cmd\": \"stop\"}", hostname="10.0.2.2")
        elif cmd[0] == 1:
            publish.single("abot/command", "{\"cmd\": \"left\", \"val\": \"" + str(cmd[1]) + "\"}", hostname="10.0.2.2")
        elif cmd[0] == 2:
            publish.single("abot/command", "{\"cmd\": \"back\", \"val\": \"" + str(cmd[1]) + "\"}", hostname="10.0.2.2")
        elif cmd[0] == 3:
            publish.single("abot/command", "{\"cmd\": \"right\", \"val\": \"" + str(cmd[1]) + "\"}", hostname="10.0.2.2")
        elif cmd[0] == 5:
            publish.single("abot/command", "{\"cmd\": \"forward\", \"val\": \"" + str(cmd[1]) + "\"}", hostname="10.0.2.2")
        elif cmd[0] == 8:
            publish.single("abot/command", json.dumps(test_json), hostname="10.0.2.2")
        else:
            break
    print("Exiting...")

def control2():
    cv2.imshow("Object Movement", np.zeros((300,500), dtype=np.uint8))
    while True:
        key = cv2.waitKey(50)
        if key == 119:
            publish.single("abot/command", "{\"cmd\": \"forward\", \"val\": \"0.5\"}", hostname="10.0.2.2")
        elif key == 97:
            publish.single("abot/command", "{\"cmd\": \"left\", \"val\": \"0.25\"}", hostname="10.0.2.2")
        elif key == 100:
            publish.single("abot/command", "{\"cmd\": \"right\", \"val\": \"0.5\"}", hostname="10.0.2.2")
        elif key == 115:
            publish.single("abot/command", "{\"cmd\": \"back\", \"val\": \"0.25\"}", hostname="10.0.2.2")

control2()
