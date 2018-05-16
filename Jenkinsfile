pipeline {
  agent none
  stages {
    stage('Linux') {
      agent {
        docker {
          image 'ubuntu:16.04'
          reuseNode false
        }

      }
      steps {
        echo 'TS3Client Pipeline'
        sh 'apt-get update && apt-get install make g++ git sudo -y && ls'
        sh 'rm -rf libtommath && git clone https://github.com/libtom/libtommath'
        sh 'make -C libtommath/ install '
        sh 'rm -rf libtomcrypt && git clone https://github.com/libtom/libtomcrypt'
        sh 'make -C libtomcrypt/ CFLAGS="-DUSE_LTM -DLTM_DESC" EXTRALIBS="-ltommath" install'
        sh 'make -f make main'
      }
    }
  }
}