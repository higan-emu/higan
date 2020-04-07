#!/usr/bin/env sh

echo "Uploading..."
name="higan-$CIRRUS_TAG-$OS.zip"
curl -X POST \
    --data-binary @higan-nightly.zip \
    --header "Authorization: token $GITHUB_TOKEN" \
    --header "Content-Type: application/octet-stream" \
    --output /dev/null \
    --write-out '%{http_code}' \
    "https://uploads.github.com/repos/$CIRRUS_REPO_FULL_NAME/releases/$CIRRUS_RELEASE/assets?name=$name"
