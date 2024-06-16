#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <iomanip>
#include <algorithm>

class HMMProfile {
private:
    std::vector<std::string> sequences;
    std::vector<int> match_states;
    std::vector<int> delete_states;
    std::unordered_map<int, std::unordered_map<char, double>> emission_probs;
    std::unordered_map<int, std::map<char, double>> transition_probs;
    std::string alphabet;

    void _build_profile() {
        // Paso 1: Determinar estados de coincidencia basados en columnas con > 50% residuos
        int alignment_length = sequences[0].length();
        for (int i = 0; i < alignment_length; ++i) {
            int non_gap_count = std::count_if(sequences.begin(), sequences.end(), [i](const std::string& seq) {
                return seq[i] != '-';
            });
            if (static_cast<double>(non_gap_count) / sequences.size() > 0.5) {
                match_states.push_back(i);
            } else {
                delete_states.push_back(i);
            }
        }

        // Paso 2: Inicializar probabilidades de transición y emisión
        _initialize_probabilities();
    }

    void _initialize_probabilities() {
        int alignment_length = sequences[0].length();

        // Inicializar probabilidades de emisión para estados de coincidencia
        for (int state : match_states) {
            emission_probs[state] = std::unordered_map<char, double>();
            for (char aa : alphabet) {
                emission_probs[state][aa] = 1.0;  // Inicialización de regla de uno más
            }
            for (const std::string& seq : sequences) {
                char aa = seq[state];
                if (aa != '-') {
                    emission_probs[state][aa] += 1.0;
                }
            }

            // Normalizar las probabilidades
            double total = 0.0;
            for (const auto& pair : emission_probs[state]) {
                total += pair.second;
            }
            for (auto& pair : emission_probs[state]) {
                pair.second /= total;
            }
        }

        // Inicializar probabilidades de transición
        for (int state = 0; state < alignment_length; ++state) {
            transition_probs[state] = {
                {'M', 1.0},
                {'I', 1.0},
                {'D', 1.0}
            };  // Inicialización de regla de uno más
            for (const std::string& seq : sequences) {
                if (seq[state] != '-') {
                    transition_probs[state]['M'] += 1.0;
                } else {
                    transition_probs[state]['D'] += 1.0;
                }
            }

            // Normalizar las probabilidades
            double total = 0.0;
            for (const auto& pair : transition_probs[state]) {
                total += pair.second;
            }
            for (auto& pair : transition_probs[state]) {
                pair.second /= total;
            }
        }
    }

public:
    HMMProfile(const std::vector<std::string>& seqs, const std::string& alpha)
        : sequences(seqs), alphabet(alpha) {
        _build_profile();
    }

    void print_model() {
        std::cout << std::fixed << std::setprecision(3); // Establecer precisión a 3 decimales
        std::cout << "Estados de Match: ";
        for (int state : match_states) {
            std::cout << state << " ";
        }
        std::cout << std::endl;

        std::cout << "Estados de Delete: ";
        for (int state : delete_states) {
            std::cout << state << " ";
        }
        std::cout << std::endl;

        std::cout << "Probabilidades de Transition: " << std::endl;
        for (const auto& pair : transition_probs) {
            std::cout << "Estado " << pair.first << ": ";
            for (const auto& trans : pair.second) {
                std::cout << trans.first << ": " << trans.second << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "Probabilidades de Emission: " << std::endl;
        for (const auto& pair : emission_probs) {
            std::cout << "Estado " << pair.first << ": ";
            for (const auto& prob : pair.second) {
                std::cout << prob.first << ": " << prob.second << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    std::vector<std::string> sequences = {
        "VGA--HAGEY",
        "V----NVDEV",
        "VEA--DVAGH",
        "VKG------D",
        "VYS--TYETS",
        "FNA--NIPKH",
        "IAGADNGAGY"
    };

    std::string alphabet = "ACDEFGHIKLMNPQRSTVWY";  // Alfabeto de aminoácidos

    HMMProfile hmm(sequences, alphabet);
    hmm.print_model();

    return 0;
}
