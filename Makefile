build:
	mkdir -p ./bin && g++ -w -std=c++17 -Wfatal-errors \
	./src/*.cpp \
	-o bin/game \
	-I"./src" \
	-I"${DEVLIB}/include" \
	-L"${DEVLIB}/lib" \
	-llua \
	-ldl \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer;

clean:
	rm ./bin/game;

run:
	./bin/game;
