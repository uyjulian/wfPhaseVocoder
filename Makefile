
SOURCES += Main.cpp PhaseVocoderFilter.cpp PhaseVocoderDSP.cpp MathAlgorithms.cpp RealFFT.cpp WaveSegmentQueue.cpp

PROJECT_BASENAME = wfPhaseVocoder

RC_LEGALCOPYRIGHT ?= Copyright (C) 2023-2023 Julian Uy; See details of license at license.txt, or the source code location.

include external/tp_stubz/Rules.lib.make
