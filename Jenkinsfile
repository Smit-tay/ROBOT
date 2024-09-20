pipeline {
    agent any

    stages {
        stage('Configure'){
            steps {
                sh '''
                rm -rf build;
                mkdir -p build;
                cd build;
                cmake ..
               '''
            }
        }
        stage('Build') {
            steps {
                echo 'Building..'
                sh '''
                   cd build
                   make -j \
                   '''

            }
        }
        stage('Test') {
            steps {
                echo 'Testing..'
            }
        }
    }
}
