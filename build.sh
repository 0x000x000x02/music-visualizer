mkdir build
g++ -o "build/main" "GLFW Stuff/main.cpp" "GLFW Stuff/glad.c" -lsfml-audio -lsfml-system -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lAquila -I"GLFW Stuff/"
mkdir "build/shaders"
cp -r "GLFW Stuff/shaders/" "build/"
