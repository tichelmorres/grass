with import <nixpkgs> { };

mkShell {
  pname = "grass toucher";

  buildInputs = [
    gcc
    libx11
  ];

  shellHook = ''
    clear
    fish
  '';
}
