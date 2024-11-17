# Run the docker container on docker image
#!/bin/bash

if [[ -z "$1" || -z "$2" ]]; then
    echo -e "\033[31;1mError: Wrong number of arguments\033[0m"
    echo -e "\033[32mUsage:\033[0m bash $0 \033[36m<image name:tag> <new container name>\033[0m"
    exit
fi

echo "Running docker container..."
docker run \
    --privileged \
    -it \
    --network host \
    --volume=$PWD:/android-cpp \
    --name "$2" \
    "$1"