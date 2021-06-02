mkdir build
g++ -o "build/main" "GLFW Stuff/main.cpp" "GLFW Stuff/glad.c" -I"./extras/linux/aquila/include" -L"./extras/linux/aquila/lib" -lsfml-audio -lsfml-system -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lAquila -I"GLFW Stuff/"
mkdir "build/shaders" && \
cp -r "GLFW Stuff/shaders/" "build/"
