#!/bin/bash

# Check if the required tools are installed
if ! command -v xdotool &> /dev/null; then
  echo "xdotool is not installed. Please install it (e.g., sudo apt-get install xdotool)."
  exit 1
fi

# Determine the active window
active_window_id=$(xdotool getactivewindow)

# Simulate pressing the "Insert" character
xdotool windowactivate $active_window_id key Insert

# Example usage (bind to Ctrl+Alt+1 in your window manager):
# In your window manager's keyboard shortcuts, create a new shortcut:
#   Command: /path/to/your/script.sh
#   Key Binding: Ctrl+Alt+1
