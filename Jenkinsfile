pipeline {
    agent any
    environment {
        GITHUB_TOKEN = credentials('github-token')
    }
    stages {
        stage('Configure Build') {
            steps {
                // Login to GHCR
                sh "echo ${GITHUB_TOKEN} | docker login ghcr.io -u matheusbosc --password-stdin"

                // Login to github cli tool
                sh 'echo "$GITHUB_TOKEN" | gh auth login --with-token || true'

                sh 'rm -f ./bin/client'
                sh 'rm -f ./bin/server'
                sh 'rm -f ./lib/libcommon_lib.a'
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

        stage('Build Server') {
                      steps {
                         // Build C++ Program
                         sh '/opt/cmake/bin/cmake --build ./cmake-build-debug --target server -- -j $(nproc)'

                         // Build Docker Image
                         sh 'docker build -t ghcr.io/matheusbosc/chat-plus-plus-server:latest .'

                         // Tag Docker Image
                         sh "docker tag ghcr.io/matheusbosc/chat-plus-plus-server:latest ghcr.io/matheusbosc/chat-plus-plus-server:${BUILD_NUMBER}"

                         // Push Docker Image
                         sh "docker push ghcr.io/matheusbosc/chat-plus-plus-server:${BUILD_NUMBER}"
                         sh 'docker push ghcr.io/matheusbosc/chat-plus-plus-server:latest'
                     }
                }

        stage('Build Client') {
              steps {
                 sh '/opt/cmake/bin/cmake --build ./cmake-build-debug --target client -- -j $(nproc)'
             }
        }

        stage('Rename Files') {
              steps {
                 sh 'mv ./bin/client ./bin/client-linux'
                 sh 'mv ./bin/server ./bin/server-linux'
             }
        }

        stage('Github Release') {
              steps {
                 sh 'ls'
                 sh 'ls bin'
                 sh 'cd bin'
                 sh 'ls'
                 sh """
                    gh release create "v${BUILD_NUMBER}" \
                    /var/lib/jenkins/jobs/Chat++\ Linux/workspace/bin/client-linux /var/lib/jenkins/jobs/Chat++\ Linux/workspace/bin/server-linux \
                    --title Build\ "${BUILD_NUMBER}" --notes Automated\ Jenkins\ release
                 """
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

