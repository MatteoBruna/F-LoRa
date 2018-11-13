PLATFORM := $(shell uname)

ifeq ($(PLATFORM), Darwin)
ARDUINO="/Applications/Arduino 2.app/Contents/MacOS/Arduino"
PORT=/dev/ttyACM0
else
ARDUINO=arduino
PORT=COM3
endif

deploy:
	$(ARDUINO) --board arduino:avr:pro:cpu=8MHzatmega328 --upload ~/Desktop/Blink/Blink.ino
