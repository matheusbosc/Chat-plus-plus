pipeline {
    agent any
    stages {
        stage('Configure Build') {
            steps {
                sh 'rm ./bin/client'
                sh 'rm ./bin/server'
                sh 'rm ./lib/libcommon_lib.a'
                sh 'rm -rf ./cmake-build-debug'
                sh 'cmake \
                      -S . \
                      -B cmake-build-debug \
                      -G Ninja \
                      -DCMAKE_BUILD_TYPE=Debug \
                      -CMAKE_EXPORT_COMPILE_COMMANDS=ON'
            }
        }

        stage('Build Common Lib') {
            steps {
                sh 'cmake --build ./cmake-build-debug --target common_lib -- -j $(nproc)'
            }
        }

        stage('Build Client') {
              steps {
                 sh 'cmake --build ./cmake-build-debug --target client -- -j $(nproc)'
             }
        }

        stage('Build Server') {
              steps {
                 sh 'cmake --build ./cmake-build-debug --target server -- -j $(nproc)'
             }
        }
    }
}

