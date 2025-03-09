ask_do sh -c '
	curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
'


ask_do cargo install cbindgen
