# Colour-RLM-Simple-Watch-w-Config

A Pebble watch face. Available on the Pebble App Store as "RLM SIMPLE COLOUR WATCH"



   44  2025-10-23 16:48:32 sudo apt install python3-pip python3-venv nodejs npm libsdl1.2debian libfdt1
   45  2025-10-23 16:50:42 curl -LsSf https://astral.sh/uv/install.sh | sh
   46  2025-10-23 16:50:53 uv tool install pebble-tool
   47  2025-10-23 16:51:08 $HOME/.local/bin/uv tool install pebble-tool
   50  2025-10-23 16:51:55 export PATH=/home/andy/.local/bin:$PATH
   51  2025-10-23 16:52:08 uv tool install pebble-tool
   52  2025-10-23 16:52:15 pebble sdk install latest
   61  2025-10-23 16:54:01 git clone git@github.com:redlegoman/Colour-RLM-Simple-Watch-w-Config.git
   70  2025-10-23 16:54:56 mv Colour-RLM-Simple-Watch-w-Config/ Colour-RLM-Simple-Watch-w-Config.saved
   71  2025-10-23 16:54:58 pebble new-project Colour-RLM-Simple-Watch-w-Config
   72  2025-10-23 16:55:01 cd Colour-RLM-Simple-Watch-w-Config
   73  2025-10-23 16:55:07 cp -rp ../Colour-RLM-Simple-Watch-w-Config.saved/. .
   75  2025-10-23 16:55:12 pebble build
  101  2025-10-23 16:59:38 pebble install --emulator basalt
  111  2025-10-23 17:03:37 pebble login
  112  2025-10-23 17:04:37 pebble install --cloudpebble
