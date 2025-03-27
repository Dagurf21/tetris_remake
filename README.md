Building with CMake

    Clone the Repository

git clone <https://github.com/yourusername/tetris_remake.git>
cd tetris_remake

Generate Build Files
Create a build directory and run CMake:

mkdir build
cd build
cmake ..

Compile the Project
Build the project with:

make

This will create the executable (e.g., tetris_remake) inside the build folder.

Run the Application
From the build directory:

    ./tetris_remake

    Assets Handling
    The CMake configuration includes a command that automatically copies the assets folder to the build directory after compilation, so the executable can find its resources.

