FILE_NAME=.vscode/settings.json
direct_read "$FILE_NAME" 512
direct_read "$FILE_NAME" 256
direct_read "$FILE_NAME" 512 1
direct_read "$FILE_NAME" 4096 8192 512
direct_read "$FILE_NAME" 4096 512 256