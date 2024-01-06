{
	inputs = {
		nixpkgs.url     = "github:nixos/nixpkgs/nixpkgs-unstable";
		flake-utils.url = "github:numtide/flake-utils";
	};

	outputs = { self, nixpkgs, flake-utils }:
		flake-utils.lib.eachDefaultSystem(system:
			let
				pkgs = nixpkgs.legacyPackages.${system};
				lib = nixpkgs.lib;
				cpp-env = with pkgs; [
					gcc13
					gnumake
					clang-tools
				];
				java-env = with pkgs; [
					jdk21
					jdt-language-server
				];
				dbg-env = with pkgs; [
					gdb
					valgrind
				];

			in rec {
				devShells.default = pkgs.mkShell {
					nativeBuildInputs = cpp-env ++ java-env ++ dbg-env;
				};
			}
		);
}
