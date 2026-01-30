pipeline {
    agent any
    stages {
        stage('Configure Build') {
            steps {
                sh 'rm -f ./bin/client'
                sh 'rm -f ./bin/server'
                sh 'rm -f ./lib/libcommon_lib.a'
                sh 'rm -rf ./cmake-build-debug'
                sh '/opt/cmake/bin/cmake \
                      -S . \
                      -B cmake-build-debug \
                      -G Ninja \
                      -DCMAKE_BUILD_TYPE=Debug \
                      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON'
            }
        }

        stage('Build Common Lib') {
            steps {
                sh '/opt/cmake/bin/cmake --build ./cmake-build-debug --target common_lib -- -j $(nproc)'
            }
        }

        stage('Build Client') {
              steps {
                 sh '/opt/cmake/bin/cmake --build ./cmake-build-debug --target client -- -j $(nproc)'
             }
        }

        stage('Build Server') {
              steps {
                 sh '/opt/cmake/bin/cmake --build ./cmake-build-debug --target server -- -j $(nproc)'
             }
        }

        stage('Final Touches') {
              steps {
                 sh 'mv ./bin/client ./bin/client-linux'
                 sh 'mv ./bin/server ./bin/server-linux'
             }
        }

        stage('Deliver') {
             steps {
                 archiveArtifacts artifacts: 'bin/client-linux', fingerprint: true
                 archiveArtifacts artifacts: 'bin/server-linux', fingerprint: true
             }
        }
    }
}

