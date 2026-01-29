pipeline {
    agent any
    stages {
        stage('Configure Build') {
            steps {
                sh 'rm -rf ./bin/client'
                sh 'rm -rf ./bin/server'
                sh 'rm -rf ./lib/libcommon_lib.a'
                sh 'rm -rf ./cmake-build-debug'
                sh 'cmake \
                      -S . \
                      -B cmake-build-debug \
                      -G Ninja \
                      -DCMAKE_BUILD_TYPE=Debug \
                      -CMAKE_EXPORT_COMPILE_COMMANDS=ON\
                      -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake'
            }
        }

        stage('Build Common Lib') {
            steps {
                sh 'cmake --build ./cmake-build-debug --target common_lib -j 6'
            }
        }

        stage('Build Client') {
              steps {
                 sh 'cmake --build ./cmake-build-debug --target client -j 6'
             }
        }

        stage('Build Server') {
              steps {
                 sh 'cmake --build ./cmake-build-debug --target server -j 6'
             }
        }
    }
}

