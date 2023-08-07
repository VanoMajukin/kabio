Name: kabio
Version: 1.0.2
Release: alt1

Summary: Kabio is a program that recognizes notes from a signal

License: GPL-3
Group: Sound
Url: https://codeberg.org/george.bartolomey/kabio

# Source-url: https://codeberg.org/george.bartolomey/%name/archive/%version.tar.gz
Source: %name-%version.tar

BuildRequires(pre): rpm-macros-cmake
BuildRequires: cmake ctest
BuildRequires: qt5-base-devel
BuildRequires: qt5-tools-devel
BuildRequires: libaubio5-devel
BuildRequires: jackit-devel

Requires: jack-audio-connection-kit

%description
Kabio is a program that recognizes notes from a signal and sends these to other program with MIDI in real time. This works with JACK Audio Connection Kit.

%prep
%setup

%build
mkdir build
pushd build
%cmake_insource .. -DCMAKE_INSTALL_PREFIX="%buildroot/usr"
%make_build
popd

%install
pushd build
%makeinstall
popd

%files
%_bindir/kabio
%_desktopdir/kabio.desktop
%_iconsdir/hicolor/48x48/apps/kabio.png
%_iconsdir/hicolor/scalable/apps/kabio.svg

%changelog
* Wed Jul 26 2023 Ivan Mazhukin <vanomj@altlinux.org> 1.0.2-alt1
- Initial build for Alt Sisyphus
