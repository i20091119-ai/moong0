# 스케치에서 받은 버튼 이름("A"/"B")을 세고 WebUI로 브라우저에 전달
from arduino.app_utils import *
from arduino.app_bricks.web_ui import WebUI

logger = Logger("button2")
ui = WebUI()

counts = {"A": 0, "B": 0}


def send_counts(event: str, last: str = ""):
    ui.send_message(event, {"counts": counts, "last": last})


# 스케치: Bridge.notify("button_pressed", "A")
def button_pressed(name: str):
    if name not in counts:
        logger.warning(f"unknown button: {name}")
        return
    counts[name] += 1
    logger.info(f"button {name} -> {counts[name]}")
    send_counts("pressed", name)


# 브라우저 초기화 버튼
def on_reset(client, data):
    counts["A"] = 0
    counts["B"] = 0
    logger.info("reset")
    send_counts("counts")


# 새로 접속한 브라우저에 현재 횟수 전달
def on_connect(sid):
    send_counts("counts")


Bridge.provide("button_pressed", button_pressed)
ui.on_message("reset", on_reset)
ui.on_connect(on_connect)
ui.expose_api("GET", "/counts", lambda: counts)

App.run()
