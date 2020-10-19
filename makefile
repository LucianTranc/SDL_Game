#game:
#	g++ main.cpp -o play -I include -L lib -l SDL2-2.0.0
CC = g++
CFLAGS = -Wall -std=c++11
INC_PATH = -I ./include/SDL2
LDFLAGS = -L ./lib -l SDL2-2.0.0 -l SDL2_image-2.0.0 -l SDL2_ttf-2.0.0

trancengine: main.o
	$(CC) $(CFLAGS) $(INC_PATH) $(LDFLAGS) bin/main.o bin/Game.o bin/TextureManager.o bin/AssetManager.o bin/Collision.o bin/GameObject.o bin/Map.o bin/Vector2D.o bin/ECS.o -o play

main.o: main.cpp bin/Game.o bin/TextureManager.o bin/AssetManager.o bin/Collision.o bin/GameObject.o bin/Map.o bin/Vector2D.o bin/ECS.o
	$(CC) $(CFLAGS) $(INC_PATH) main.cpp -c -o bin/main.o

bin/Game.o: Game.cpp Game.h
	$(CC) $(CFLAGS) $(INC_PATH) Game.cpp -c -o bin/Game.o

bin/TextureManager.o: TextureManager.cpp TextureManager.h
	$(CC) $(CFLAGS) $(INC_PATH) TextureManager.cpp -c -o bin/TextureManager.o

bin/AssetManager.o: AssetManager.cpp AssetManager.h
	$(CC) $(CFLAGS) $(INC_PATH) AssetManager.cpp -c -o bin/AssetManager.o

bin/Collision.o: Collision.cpp Collision.h
	$(CC) $(CFLAGS) $(INC_PATH) Collision.cpp -c -o bin/Collision.o

bin/GameObject.o: GameObject.cpp GameObject.h
	$(CC) $(CFLAGS) $(INC_PATH) GameObject.cpp -c -o bin/GameObject.o

bin/Map.o: Map.cpp Map.h
	$(CC) $(CFLAGS) $(INC_PATH) Map.cpp -c -o bin/Map.o

bin/Vector2D.o: Vector2D.cpp Vector2D.h
	$(CC) $(CFLAGS) $(INC_PATH) Vector2D.cpp -c -o bin/Vector2D.o

bin/ECS.o: ECS/ECS.cpp ECS/ECS.h ECS/Components.h ECS/Animation.h ECS/ColliderComponent.h ECS/KeyboardController.h ECS/ProjectileComponent.h ECS/SpriteComponent.h ECS/TileComponent.h ECS/TransformComponent.h ECS/UILabel.h
	$(CC) $(CFLAGS) $(INC_PATH) ECS/ECS.cpp -c -o bin/ECS.o

clean :
	-rm *.o bin/*.o play
