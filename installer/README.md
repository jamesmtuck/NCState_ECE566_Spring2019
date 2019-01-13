To create an installation equivalent to VCL, use the cmake-install-llvm.sh script.
+ Open the file.
+ Specify the build version of LLVM and the install directory.
+ Read through the installation steps to make sure you want everything it will do.
+ Make sure you have the necessary pre-reqs installed.  For VCL on a base Ubuntu 18.04 image, I needed to do the following:
    - sudo apt-get install cmake ninja-build libtool flex bison libbison-dev git python binutils graphviz doxygen emacs
    - Some of these tools may not be necessary, in particular the last four, although it is handy to have a text editor available
      in case something goes wrong.
+ mkdir tmp
+ cd tmp
+ ../cmake-install-llvm.sh


To create your own docker image that matches mine, do the following:
+ Install Docker on your system and set up an account.
+ cd ./docker
+ From a command line:
    - ./build_docker_image.sh -b tags/RELEASE_701/final -s ubuntu -d ubuntu-llvm7.0.1-release -t "latest" -p clang -i stage2-install -i stage2-install-clang -i stage2-install-clang-headers -- -DCMAKE_BUILD_TYPE=Release -DBOOTSTRAP_CMAKE_BUILD_TYPE=Release -DCLANG_ENABLE_BOOTSTRAP=ON -DCLANG_BOOTSTRAP_TARGETS="install-clang;install-clang-headers" -DLLVM_INCLUDE_TOOLS=ON -DLLVM_OPTIMIZED_TABLEGEN=ON
    - This build will take a while, so you be sure to run on powerful enough system and use nohup.
+ If all went well, you now have an image called ubuntu-llvm7.0.1-release on your system. If you want to share it with others:
    - docker tag ubuntu-llvm7.0.1-release docker-username/image-name
    - docker push docker-username/image-name
