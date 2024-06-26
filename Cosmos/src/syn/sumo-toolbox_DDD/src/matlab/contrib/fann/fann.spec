Summary:    A fast artificial neural network library.
Name:       fann
Version:    2.2.0
Release:    1
License:    LGPL
Group:      Development/Libraries
URL:        http://fann.sf.net/
Source:     http://prdownloads.sourceforge.net/fann/%{name}-%{version}.tar.bz2
BuildRoot:  %{_tmppath}/%{name}-%{version}-root

%package devel
Summary: Development libraries for FANN
Requires: fann = 2.2.0-%{release}
Group: Development/Libraries

%description
Fast Artificial Neural Network (FANN) Library is written in ANSI C. The library implements multilayer feedforward ANNs, up to 150 times faster than other libraries. FANN supports execution in fixed point, for fast execution on systems like the iPAQ.

%description devel
This package is only needed if you intend to develop and/or compile programs based on the FANN library.

%prep
%setup -q
./configure --prefix=/usr/local

%build
make
(cd doc && make html-single)

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_libdir}/libdoublefann.so
%{_libdir}/libdoublefann.so.1
%{_libdir}/libdoublefann.so.2.2.0
%{_libdir}/libfloatfann.so
%{_libdir}/libfloatfann.so.1
%{_libdir}/libfloatfann.so.2.2.0
%{_libdir}/libfixedfann.so
%{_libdir}/libfixedfann.so.1
%{_libdir}/libfixedfann.so.2.2.0
%{_libdir}/libfann.so
%{_libdir}/libfann.so.1
%{_libdir}/libfann.so.2.2.0

%files devel
%doc AUTHORS ChangeLog COPYING INSTALL NEWS README TODO doc/fann.html
%{_libdir}/pkgconfig/fann.pc
%{_libdir}/libdoublefann.a
%{_libdir}/libdoublefann.la
%{_libdir}/libfloatfann.a
%{_libdir}/libfloatfann.la
%{_libdir}/libfixedfann.a
%{_libdir}/libfixedfann.la
%{_libdir}/libfann.a
%{_libdir}/libfann.la
%{_includedir}/fann.h
%{_includedir}/doublefann.h
%{_includedir}/fann_internal.h
%{_includedir}/floatfann.h
%{_includedir}/fann_data.h
%{_includedir}/fixedfann.h
%{_includedir}/compat_time.h
%{_includedir}/fann_errno.h
%{_includedir}/fann_activation.h

%changelog
* Fri Mar 30 2004 Evan Nemerson <evan@coeus-group.com>
- Build and install HTML documentation.

* Fri Jan 16 2004 Evan Nemerson <evan@coeus-group.com>
- Added activation header.

* Mon Jan 10 2004 Evan Nemerson <evan@coeus-group.com>
- RPM created
