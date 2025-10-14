pipeline {
    agent none

    options {
        buildDiscarder(logRotator(numToKeepStr: '10'))
        disableConcurrentBuilds()
        timeout(time: 10, unit: 'MINUTES')
        timestamps()
    }

    // Check for Git changes every 5 minutes
    triggers {
        pollSCM('H/5 * * * *')
    }

    stages {
        // === macOS main build stage ===
        stage('Build macOS') {
            agent { label 'macos12' }
            environment {
                CUSTOM_WORKSPACE = 'workspace/djnn-qt/djnn-cpp'
            }
            steps {
                dir(env.CUSTOM_WORKSPACE) {
                    cleanWs()
                    checkout([
                        $class: 'GitSCM',
                        branches: [[name: '*/pipeline']], // <-- branch "pipeline"
                        userRemoteConfigs: [[
                            url: env.DJNN_CPP_URL,          // externalized variable
                            credentialsId: env.GIT_CREDENTIAL_ID // externalized Jenkins credential ID
                        ]]
                    ])
                    // Generate the minimal config.mk
                    writeFile file: 'config.mk', text: 'use_ivy := yes\n'

                    sh '''
                        echo "Starting macOS build"
                        export PATH="/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"
                        make -j V=max
                    '''
                }
            }
            post {
                failure {
                    emailext(
                        to: "${env.EMAIL1}, ${env.EMAIL2}",
                        subject: "❌ macOS build failed - djnn-cpp #${currentBuild.number}",
                        body: "The macOS build failed.\nDetails: ${env.BUILD_URL}"
                    )
                }
            }
        }

        // === Ubuntu & Windows parallel builds ===
        stage('Build Ubuntu & Windows') {
            when {
                expression { currentBuild.result == null || currentBuild.result == 'SUCCESS' }
            }
            parallel {
                stage('Build Ubuntu') {
                    agent { label 'ubuntu' }
                    environment {
                        CUSTOM_WORKSPACE = 'workspace/djnn-qt/djnn-cpp'
                    }
                    steps {
                        dir(env.CUSTOM_WORKSPACE) {
                            cleanWs()
                            checkout([
                                $class: 'GitSCM',
                                branches: [[name: '*/pipeline']],
                                userRemoteConfigs: [[
                                    url: env.DJNN_CPP_URL,
                                    credentialsId: env.GIT_CREDENTIAL_ID
                                ]]
                            ])
                            writeFile file: 'config.mk', text: 'use_ivy := yes\n'
                            sh 'make -j8 V=max'
                        }
                    }
                    post {
                        failure {
                            emailext(
                                to: "${env.EMAIL1}, ${env.EMAIL2}",
                                subject: "❌ Ubuntu build failed - djnn-cpp #${currentBuild.number}",
                                body: "The Ubuntu build failed.\nDetails: ${env.BUILD_URL}"
                            )
                        }
                    }
                }

                stage('Build Windows') {
                    agent { label 'win10' }
                    environment {
                        CUSTOM_WORKSPACE = 'workspace/djnn-qt/djnn-cpp'
                    }
                    steps {
                        dir(env.CUSTOM_WORKSPACE) {
                            cleanWs()
                            checkout([
                                $class: 'GitSCM',
                                branches: [[name: '*/pipeline']],
                                userRemoteConfigs: [[
                                    url: env.DJNN_CPP_URL,
                                    credentialsId: env.GIT_CREDENTIAL_ID
                                ]]
                            ])
                            bat '''
                                make config_qt
                                echo use_ivy := yes > config.mk
                                make -j V=max
                            '''
                        }
                    }
                    post {
                        failure {
                            emailext(
                                to: "${env.EMAIL1}, ${env.EMAIL2}",
                                subject: "❌ Windows build failed - djnn-cpp #${currentBuild.number}",
                                body: "The Windows build failed.\nDetails: ${env.BUILD_URL}"
                            )
                        }
                    }
                }
            }
        }
    }

    // === Global post actions ===
    post {
    success {
        script {

            try {
                // Trigger smala only if full pipeline succeeded
                build job: 'smala', wait: false
            } catch (Exception e) {
                echo "⚠️ Unable to trigger smala: ${e.message}"
            }

            if (currentBuild.previousBuild?.result == 'FAILURE') {
                emailext(
                    to: "${env.EMAIL1}, ${env.EMAIL2}",
                    subject: "✅ Back to normal - djnn-cpp #${currentBuild.number}",
                    body: """
The build has recovered after a failure.

Previous build: #${currentBuild.previousBuild.number} (${currentBuild.previousBuild.result})
Current build: SUCCESS

Details: ${env.BUILD_URL}
"""
                )
            }
        }
    }

    failure {
        emailext(
            to: "${env.EMAIL1}, ${env.EMAIL2}",
            subject: "❌ Build failed - djnn-cpp #${currentBuild.number}",
            body: """
The build has failed.

Details: ${env.BUILD_URL}

Error (last 100 lines):
${currentBuild.rawBuild.getLog(100).join('\n')}

"""
        )
    }
}
}
