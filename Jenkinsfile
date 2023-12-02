#!/usr/bin/env groovy

pipeline {

    agent {
        docker {
            image 'mcr.microsoft.com/devcontainers/cpp'
            args '-u root'
        }
    }

    stages {
        stage('Build') {
            steps {
                echo 'Building...'
                sh 'cmake . -B build && make -C build/ testMathmart'
            }
        }
        stage('Test') {
            steps {
                echo 'Testing...'
                sh './build/testMathmart'
            }
        }
    }
}
