#include <iostream>
#include <sstream>
#include "../include/Export.h"

const int NUM_PROTEIN_INIT = 0;
const int MIN_DISSOCIATION_SIZE = 60;
const int INSERTION_MULTIPLIER = 30;
const int DISSOCIATION_INTERVAL = 2500;
const int EXPORT_INTERVAL = 500;
const int CLUMP_START_SIZE = 10;
const int CLUMP_MIN_SIZE = 3;
const int MAX_PROTEINS = 20000;
const int LATTICE_SIZE = 800;

const double R_I = 0.2;
const double R_D = 0.5;
const double DELTA_T = 0.0002328;

const bool EXPORT_LATTICE = 1;
const bool EXPORT_HISTOGRAM = 0;
const bool EXPORT_CLUMP = 0;
const bool EXPORT_AMAX = 0;

double gK;
int gIterations;
int gCodeItr;
std::string gOutputPath("");


bool ParseArguments(int argc, const char* argv[])
{
    int paramNum = 2; //Min number of parameters

    if (argc < paramNum * 2)
    {
        std::cout << "Not enough cmd line inputs" << std::endl;
    }
    else
    {
        for (int i = 1; i < argc;)
        {
            if (strcmp(argv[i], "-k") == 0)
            {
                gK = std::stod(argv[i + 1]);
                i += 2;
            }
            else if (strcmp(argv[i], "-i") == 0)
            {
                gIterations = std::atoi(argv[i + 1]);
                i += 2;
            }
            else if (strcmp(argv[i], "-n") == 0)
            {
                gCodeItr = std::atoi(argv[i + 1]);
                i += 2;
            }
            else if (strcmp(argv[i], "-o") == 0)
            {
                gOutputPath = argv[i + 1];
                i += 2;
            }
            else
            {
                std::cout << "Invalid input parameter" << std::endl;
                return false;
            }
        }
    }

    return true;
}

int main(int argc, const char* argv[])
{
    if (!ParseArguments(argc, argv))
    {
        std::cout << "Arguments could not be parsed";
        return 0;
    }

    Lattice lattice = Lattice(LATTICE_SIZE, NUM_PROTEIN_INIT, INSERTION_MULTIPLIER, R_I, DELTA_T, gK, MAX_PROTEINS);
    Export exp = Export(&lattice, gOutputPath, gCodeItr, gK, EXPORT_LATTICE, EXPORT_HISTOGRAM, EXPORT_CLUMP, EXPORT_AMAX);
    exp.WriteParameters(MIN_DISSOCIATION_SIZE, INSERTION_MULTIPLIER, CLUMP_START_SIZE, CLUMP_MIN_SIZE, R_I, R_D, DELTA_T, gK, gIterations);

    for (int itr = 0; itr < gIterations; itr++)
    {
        std::cout << "Itr: " << itr << std::endl;

        if (itr % EXPORT_INTERVAL == 0)
        {
            exp.Run(itr);
        }

        lattice.CheckInsertion();

        lattice.RunIteration();
    }

    exp.Run(gIterations);
}
