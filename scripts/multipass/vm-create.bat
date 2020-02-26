
REM https://multipass.run/

multipass launch --name mulubucpp --disk 30G --mem 4G --cpus 4 20.04
multipass mount "%USERPROFILE%" mulubucpp:/mnt/win_user
multipass mount "%~dp0..\.." mulubucpp:/mnt/win_repo
multipass exec mulubucpp -- mkdir -p /home/ubuntu/DEV
multipass exec mulubucpp -- cp /mnt/win_repo/scripts/multipass/provision-software.sh /home/ubuntu/DEV/provision-software.sh
multipass exec mulubucpp -- chmod 700 /home/ubuntu/DEV/provision-software.sh
multipass exec mulubucpp -- sudo apt-get update
multipass exec mulubucpp -- sudo apt install -y dos2unix
multipass exec mulubucpp -- dos2unix /home/ubuntu/DEV/provision-software.sh
multipass exec mulubucpp -- /home/ubuntu/DEV/provision-software.sh
multipass info mulubucpp
