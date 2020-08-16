
vCILENCE ?= ERROR
ifeq ($(vCILENCE), 1)
    vCILENCE := @
else
    vCILENCE :=
endif

vPRJPATH = .

vTARGET = $(vPRJPATH)/bin/image_self

vCCPREFIX = 
CC = $(vCCPREFIX)gcc
#CC = $(vCCPREFIX)g++

vSOURCE  = $(vPRJPATH)/src/*.c
#vSOURCE += $(vPRJPATH)/src/*.cpp

vINCLUDE  = -I$(vPRJPATH)/include/
vINCLUDE += -I$(vPRJPATH)/include/stb_image/

vLIB  = -lstdc++
vLIB +=

vLIBPATH  = 


all:
	$(vCILENCE)$(CC) $(vSOURCE) -o $(vTARGET) $(vINCLUDE) $(vLIB)
	@echo "oh yeah, build successfully!"

clean:
	$(vCILENCE)rm -rf $(vTARGET)
