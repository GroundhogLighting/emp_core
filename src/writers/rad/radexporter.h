#pragma once

class RadExporter {
private:
	std::string exportDir;

public:
	RadExporter();
	~RadExporter();
	void setExportDir(std::string outDir);
	bool exportModel(GroundhogModel * model, bool verbose);
	bool writeLayers(GroundhogModel * model, std::string exportDir);

	void writeClosedFace(std::ofstream * file, Face * face);
	void writeFace(std::ofstream * file, Face * face);
	void writeLoop(std::ofstream * file, Loop * loop);

};