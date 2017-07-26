#pragma once

class RadExporter {
private:
	std::string exportDir;

public:
	RadExporter();
	~RadExporter();

	bool writeViews(GroundhogModel * model, std::string exportDir);
	bool exportModel(GroundhogModel * model, std::string exportDir, bool verbose);
	bool writeLayers(GroundhogModel * model, std::string exportDir);
	void writeClosedFace(std::ofstream * file, Face * face);
	void writeFace(std::ofstream * file, Face * face);
	void writeComponentInstance(std::ofstream * file, ComponentInstance * instance);
	void writeLoop(std::ofstream * file, Loop * loop);
	bool writeComponentDefinitions(GroundhogModel * model, std::string exportDir);
	void writeModelInfo(GroundhogModel * model, std::string exportDir);

};