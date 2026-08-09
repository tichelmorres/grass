with import <nixpkgs> { };

mkShell {
  pname = "grass toucher";

  buildInputs = [
    gcc        # the compiler
    libx11     # we use X11 for unix
    libxext    # for MIT-SHM
  ];

  shellHook = ''
    clear
    fish
  '';
}
