#!/bin/bash

# Variables
EMAIL="aminebenmahdi@ecole2600.com"
KEY_PATH="$HOME/.ssh/id_ed25519"
REPO_URL="git@github.com:aminebenmahdi/ari.git"

# Step 1: Generate SSH Key
echo "Generating SSH key..."
if [ ! -f "$KEY_PATH" ]; then
  ssh-keygen -t ed25519 -C "$EMAIL" -f "$KEY_PATH" -N ""
else
  echo "SSH key already exists at $KEY_PATH"
fi

# Step 2: Start ssh-agent and add SSH key
echo "Starting ssh-agent..."
eval "$(ssh-agent -s)"
ssh-add "$KEY_PATH"

# Step 3: Display public key and copy it to clipboard
echo "Copying SSH public key to clipboard..."
if command -v xclip > /dev/null; then
  xclip -sel clip < "${KEY_PATH}.pub"
  echo "Public key copied to clipboard (requires xclip)."
elif command -v pbcopy > /dev/null; then
  pbcopy < "${KEY_PATH}.pub"
  echo "Public key copied to clipboard (requires pbcopy)."
else
  echo "Please copy the following public key manually:"
  cat "${KEY_PATH}.pub"
fi

# Step 4: Instructions for adding the key to GitHub
echo ""
echo "Please go to GitHub and add the copied key:"
echo "1. Log in to GitHub."
echo "2. Go to Settings > SSH and GPG keys."
echo "3. Click New SSH key."
echo "4. Paste the key from your clipboard."
echo "5. Click Add SSH key."
echo ""

# Step 5: Set up the remote repository to use SSH
echo "Setting up Git remote repository..."
git remote set-url origin "$REPO_URL"

echo "Setup complete!"
