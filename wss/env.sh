export CARGO_HOME=/data/cargo
export RUSTUP_HOME=/data/rustup

# http bind address (addy on the ssl cert too)
export BIND="$(cat /data/openpilot/wss/cfg.c2.ip)"
export HOST="${BIND}"
