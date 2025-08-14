# Real-world Usage

This page showcases production-ready applications and patterns using the Modern C++ Calculator library. These examples demonstrate how to integrate the calculator into real-world systems, handle edge cases, and build maintainable, scalable solutions.

## Configuration Management System

### Dynamic Configuration Engine

Build a configuration system that evaluates formulas dynamically:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <regex>
#include <chrono>

class ConfigurationEngine {
private:
    calculator::ExpressionParser<double> parser_;
    std::unordered_map<std::string, std::string> formulas_;
    std::unordered_map<std::string, double> cached_values_;
    std::unordered_map<std::string, std::chrono::system_clock::time_point> cache_timestamps_;
    std::chrono::seconds cache_ttl_;
    
public:
    ConfigurationEngine(std::chrono::seconds cache_ttl = std::chrono::seconds(300)) 
        : cache_ttl_(cache_ttl) {
        setup_environment();
    }
    
    void setup_environment() {
        // System constants
        parser_.set("pi", M_PI);
        parser_.set("e", M_E);
        
        // Time-based variables
        auto now = std::chrono::system_clock::now();
        auto epoch = now.time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
        parser_.set("current_time", static_cast<double>(seconds.count()));
        
        // System information (simplified)
        parser_.set("cpu_cores", static_cast<double>(std::thread::hardware_concurrency()));
        parser_.set("memory_gb", 16.0); // Would be dynamically determined in real system
        
        // Mathematical functions
        parser_.set("min", [](double x) { return std::min(x, 100.0); });
        parser_.set("max", [](double x) { return std::max(x, 0.0); });
        parser_.set("clamp", [](double x) { return std::max(0.0, std::min(x, 100.0)); });
        parser_.set("round", [](double x) { return std::round(x); });
        parser_.set("ceil", [](double x) { return std::ceil(x); });
        parser_.set("floor", [](double x) { return std::floor(x); });
    }
    
    void load_configuration(const std::string& config_data) {
        std::istringstream stream(config_data);
        std::string line;
        
        while (std::getline(stream, line)) {
            // Skip comments and empty lines
            if (line.empty() || line[0] == '#') continue;
            
            // Parse key = formula
            std::regex config_regex(R"(^\s*(\w+)\s*=\s*(.+)\s*$)");
            std::smatch match;
            
            if (std::regex_match(line, match, config_regex)) {
                std::string key = match[1].str();
                std::string formula = match[2].str();
                formulas_[key] = formula;
            }
        }
    }
    
    double get_value(const std::string& key) {
        // Check cache first
        auto cache_it = cached_values_.find(key);
        auto timestamp_it = cache_timestamps_.find(key);
        
        if (cache_it != cached_values_.end() && timestamp_it != cache_timestamps_.end()) {
            auto now = std::chrono::system_clock::now();
            if (now - timestamp_it->second < cache_ttl_) {
                return cache_it->second;
            }
        }
        
        // Evaluate formula
        auto formula_it = formulas_.find(key);
        if (formula_it == formulas_.end()) {
            throw std::runtime_error("Configuration key not found: " + key);
        }
        
        try {
            double value = parser_.eval(formula_it->second);
            
            // Cache the result
            cached_values_[key] = value;
            cache_timestamps_[key] = std::chrono::system_clock::now();
            
            return value;
        } catch (const calculator::error& e) {
            throw std::runtime_error("Error evaluating formula for '" + key + "': " + e.what());
        }
    }
    
    void set_variable(const std::string& name, double value) {
        parser_.set(name, value);
        // Clear cache when variables change
        cached_values_.clear();
        cache_timestamps_.clear();
    }
    
    void update_system_variables() {
        // Update time-based variables
        auto now = std::chrono::system_clock::now();
        auto epoch = now.time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
        parser_.set("current_time", static_cast<double>(seconds.count()));
        
        // Update other system metrics (CPU usage, memory, etc.)
        // This would interface with system APIs in a real implementation
    }
    
    std::vector<std::string> list_configurations() const {
        std::vector<std::string> keys;
        for (const auto& pair : formulas_) {
            keys.push_back(pair.first);
        }
        return keys;
    }
    
    void clear_cache() {
        cached_values_.clear();
        cache_timestamps_.clear();
    }
};

void demonstrate_configuration_system() {
    ConfigurationEngine config;
    
    // Sample configuration
    std::string config_data = R"(
# Server Configuration
max_connections = min(cpu_cores * 100, 1000)
timeout_seconds = max(30, memory_gb * 2)
buffer_size = round(memory_gb * 1024 * 0.1)

# Load Balancing
load_threshold = clamp(cpu_cores * 15)
scale_factor = memory_gb / 8

# Cache Settings
cache_size_mb = floor(memory_gb * 1024 * 0.25)
cache_ttl_minutes = max(5, memory_gb)

# Rate Limiting
requests_per_second = cpu_cores * 50
burst_limit = requests_per_second * 2
)";
    
    config.load_configuration(config_data);
    
    std::cout << "=== Configuration Management System ===" << std::endl;
    
    // Set runtime variables
    config.set_variable("current_load", 75.0);
    config.set_variable("active_users", 150.0);
    
    // Retrieve configuration values
    auto configs = config.list_configurations();
    for (const auto& key : configs) {
        try {
            double value = config.get_value(key);
            std::cout << key << " = " << value << std::endl;
        } catch (const std::exception& e) {
            std::cout << key << " = ERROR: " << e.what() << std::endl;
        }
    }
    
    // Demonstrate caching
    std::cout << "\nCache performance test:" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        config.get_value("max_connections");
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto cached_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    config.clear_cache();
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        config.get_value("max_connections");
    }
    end = std::chrono::high_resolution_clock::now();
    auto uncached_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Cached: " << cached_time.count() << " μs" << std::endl;
    std::cout << "Uncached: " << uncached_time.count() << " μs" << std::endl;
    std::cout << "Speedup: " << (double)uncached_time.count() / cached_time.count() << "x" << std::endl;
}
```

## Game Development: Damage System

### Advanced Damage Calculation Engine

Implement a flexible damage system for games:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <string>

class GameDamageSystem {
private:
    calculator::ExpressionParser<double> parser_;
    std::unordered_map<std::string, std::string> damage_formulas_;
    std::mt19937 rng_;
    
public:
    GameDamageSystem() : rng_(std::random_device{}()) {
        setup_game_functions();
        setup_damage_formulas();
    }
    
    void setup_game_functions() {
        // Mathematical functions
        parser_.set("min", [](double x) { return std::min(x, 9999.0); });
        parser_.set("max", [](double x) { return std::max(x, 0.0); });
        parser_.set("clamp", [](double x) { return std::max(0.0, std::min(x, 9999.0)); });
        parser_.set("round", [](double x) { return std::round(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("pow", [](double x) { return x; }); // Simplified, would need two parameters
        
        // Game-specific functions
        parser_.set("crit_multiplier", [this](double crit_chance) {
            std::uniform_real_distribution<double> dist(0.0, 100.0);
            return dist(rng_) < crit_chance ? 2.0 : 1.0;
        });
        
        parser_.set("random", [this](double max_val) {
            std::uniform_real_distribution<double> dist(0.0, max_val);
            return dist(rng_);
        });
        
        parser_.set("resistance_factor", [](double resistance) {
            return std::max(0.0, 1.0 - resistance / 100.0);
        });
        
        parser_.set("level_scaling", [](double level) {
            return 1.0 + (level - 1) * 0.1; // 10% increase per level
        });
    }
    
    void setup_damage_formulas() {
        // Physical damage formula
        damage_formulas_["physical"] = 
            "round(max(1, (base_damage + weapon_damage) * level_scaling(attacker_level) * "
            "crit_multiplier(crit_chance) * resistance_factor(target_armor) * "
            "(1 + random(0.2) - 0.1)))"; // ±10% variance
        
        // Magical damage formula
        damage_formulas_["magical"] = 
            "round(max(1, (spell_power + intelligence * 0.5) * level_scaling(attacker_level) * "
            "crit_multiplier(crit_chance) * resistance_factor(target_magic_resist) * "
            "(1 + random(0.15) - 0.075)))"; // ±7.5% variance
        
        // True damage (ignores resistances)
        damage_formulas_["true"] = 
            "round(max(1, base_damage * level_scaling(attacker_level) * "
            "crit_multiplier(crit_chance)))";
        
        // Damage over time
        damage_formulas_["dot"] = 
            "round(max(1, dot_base * level_scaling(attacker_level) * "
            "resistance_factor(target_magic_resist) * dot_stacks))";
        
        // Healing formula
        damage_formulas_["heal"] = 
            "round(max(1, heal_power * level_scaling(caster_level) * "
            "(1 + random(0.1) - 0.05)))"; // ±5% variance
    }
    
    struct DamageResult {
        double damage;
        bool critical;
        std::string damage_type;
        std::vector<std::string> modifiers;
    };
    
    DamageResult calculate_damage(const std::string& damage_type, 
                                 const std::unordered_map<std::string, double>& parameters) {
        // Set all parameters
        for (const auto& [key, value] : parameters) {
            parser_.set(key, value);
        }
        
        // Find damage formula
        auto formula_it = damage_formulas_.find(damage_type);
        if (formula_it == damage_formulas_.end()) {
            throw std::runtime_error("Unknown damage type: " + damage_type);
        }
        
        // Calculate damage
        double damage = parser_.eval(formula_it->second);
        
        // Determine if critical hit occurred
        double crit_chance = parameters.count("crit_chance") ? parameters.at("crit_chance") : 0.0;
        std::uniform_real_distribution<double> crit_dist(0.0, 100.0);
        bool critical = crit_dist(rng_) < crit_chance;
        
        // Collect modifiers that were applied
        std::vector<std::string> modifiers;
        if (critical) modifiers.push_back("Critical Hit");
        if (parameters.count("target_armor") && parameters.at("target_armor") > 0) {
            modifiers.push_back("Armor Reduction");
        }
        if (parameters.count("target_magic_resist") && parameters.at("target_magic_resist") > 0) {
            modifiers.push_back("Magic Resistance");
        }
        
        return {damage, critical, damage_type, modifiers};
    }
    
    void simulate_combat() {
        std::cout << "=== Game Damage System Simulation ===" << std::endl;
        
        // Character stats
        std::unordered_map<std::string, double> attacker_stats = {
            {"attacker_level", 15},
            {"base_damage", 50},
            {"weapon_damage", 25},
            {"spell_power", 80},
            {"intelligence", 40},
            {"crit_chance", 15.0}, // 15% crit chance
            {"heal_power", 60}
        };
        
        std::unordered_map<std::string, double> target_stats = {
            {"target_armor", 30.0}, // 30% physical damage reduction
            {"target_magic_resist", 20.0}, // 20% magic damage reduction
            {"caster_level", 12}
        };
        
        // Combine stats
        std::unordered_map<std::string, double> combat_params = attacker_stats;
        combat_params.insert(target_stats.begin(), target_stats.end());
        
        // Simulate different damage types
        std::vector<std::string> damage_types = {"physical", "magical", "true"};
        
        for (const auto& type : damage_types) {
            std::cout << "\n" << type << " damage simulation (10 attacks):" << std::endl;
            
            double total_damage = 0.0;
            int critical_hits = 0;
            
            for (int i = 0; i < 10; ++i) {
                auto result = calculate_damage(type, combat_params);
                total_damage += result.damage;
                if (result.critical) critical_hits++;
                
                std::cout << "Attack " << (i+1) << ": " << result.damage << " damage";
                if (result.critical) std::cout << " (CRITICAL!)";
                if (!result.modifiers.empty()) {
                    std::cout << " [";
                    for (size_t j = 0; j < result.modifiers.size(); ++j) {
                        if (j > 0) std::cout << ", ";
                        std::cout << result.modifiers[j];
                    }
                    std::cout << "]";
                }
                std::cout << std::endl;
            }
            
            std::cout << "Average damage: " << (total_damage / 10.0) << std::endl;
            std::cout << "Critical hit rate: " << (critical_hits * 10) << "%" << std::endl;
        }
        
        // Damage over time simulation
        std::cout << "\nDamage over time simulation:" << std::endl;
        combat_params["dot_base"] = 15.0;
        
        for (int stack = 1; stack <= 5; ++stack) {
            combat_params["dot_stacks"] = stack;
            auto result = calculate_damage("dot", combat_params);
            std::cout << "DoT Stack " << stack << ": " << result.damage << " damage/tick" << std::endl;
        }
        
        // Healing simulation
        std::cout << "\nHealing simulation:" << std::endl;
        for (int i = 0; i < 5; ++i) {
            auto result = calculate_damage("heal", combat_params);
            std::cout << "Heal " << (i+1) << ": " << result.damage << " HP restored" << std::endl;
        }
    }
};

void demonstrate_game_damage_system() {
    GameDamageSystem damage_system;
    damage_system.simulate_combat();
}
```

## Scientific Data Analysis

### Laboratory Data Processing System

Build a system for processing and analyzing scientific measurements:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <cmath>

class ScientificDataProcessor {
private:
    calculator::ExpressionParser<double> parser_;
    std::vector<std::vector<double>> datasets_;
    std::unordered_map<std::string, std::string> analysis_formulas_;
    
public:
    ScientificDataProcessor() {
        setup_scientific_functions();
        setup_analysis_formulas();
    }
    
    void setup_scientific_functions() {
        // Mathematical functions
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("log", [](double x) { return std::log(x); });
        parser_.set("log10", [](double x) { return std::log10(x); });
        parser_.set("exp", [](double x) { return std::exp(x); });
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        parser_.set("abs", [](double x) { return std::abs(x); });
        parser_.set("round", [](double x) { return std::round(x); });
        
        // Statistical functions
        parser_.set("min", [](double x) { return x; }); // Simplified
        parser_.set("max", [](double x) { return x; }); // Simplified
        
        // Constants
        parser_.set("pi", M_PI);
        parser_.set("e", M_E);
        parser_.set("avogadro", 6.02214076e23);
        parser_.set("planck", 6.62607015e-34);
        parser_.set("c", 299792458.0); // Speed of light
        parser_.set("k_boltzmann", 1.380649e-23);
        parser_.set("gas_constant", 8.314462618);
    }
    
    void setup_analysis_formulas() {
        // Statistical formulas
        analysis_formulas_["mean"] = "sum / n";
        analysis_formulas_["variance"] = "sum_squares / n - (sum / n) ** 2";
        analysis_formulas_["std_dev"] = "sqrt(sum_squares / n - (sum / n) ** 2)";
        analysis_formulas_["cv"] = "sqrt(sum_squares / n - (sum / n) ** 2) / (sum / n) * 100";
        analysis_formulas_["sem"] = "sqrt(sum_squares / n - (sum / n) ** 2) / sqrt(n)";
        
        // Chemistry formulas
        analysis_formulas_["molarity"] = "moles / volume_l";
        analysis_formulas_["ph"] = "-log10(h_concentration)";
        analysis_formulas_["pka"] = "-log10(ka)";
        analysis_formulas_["henderson_hasselbalch"] = "pka + log10(base_conc / acid_conc)";
        
        // Physics formulas
        analysis_formulas_["kinetic_energy"] = "0.5 * mass * velocity ** 2";
        analysis_formulas_["ideal_gas_pressure"] = "n * gas_constant * temperature / volume";
        analysis_formulas_["wavelength"] = "c / frequency";
        analysis_formulas_["photon_energy"] = "planck * frequency";
        
        // Error analysis
        analysis_formulas_["relative_error"] = "abs(measured - true_value) / true_value * 100";
        analysis_formulas_["z_score"] = "(value - mean) / std_dev";
        analysis_formulas_["confidence_interval"] = "mean + z_critical * std_dev / sqrt(n)";
    }
    
    void load_dataset(const std::vector<double>& data, const std::string& name = "") {
        datasets_.push_back(data);
        
        // Calculate basic statistics
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        double sum_squares = std::accumulate(data.begin(), data.end(), 0.0,
            [](double acc, double val) { return acc + val * val; });
        
        parser_.set("n", static_cast<double>(data.size()));
        parser_.set("sum", sum);
        parser_.set("sum_squares", sum_squares);
        
        // Set min/max
        auto [min_it, max_it] = std::minmax_element(data.begin(), data.end());
        parser_.set("data_min", *min_it);
        parser_.set("data_max", *max_it);
    }
    
    double analyze(const std::string& analysis_type) {
        auto formula_it = analysis_formulas_.find(analysis_type);
        if (formula_it == analysis_formulas_.end()) {
            throw std::runtime_error("Unknown analysis type: " + analysis_type);
        }
        
        return parser_.eval(formula_it->second);
    }
    
    void set_experimental_parameters(const std::unordered_map<std::string, double>& params) {
        for (const auto& [key, value] : params) {
            parser_.set(key, value);
        }
    }
    
    struct QualityControlResult {
        bool passed;
        std::vector<std::string> issues;
        double quality_score;
    };
    
    QualityControlResult quality_control_check(const std::vector<double>& data) {
        load_dataset(data);
        
        QualityControlResult result;
        result.passed = true;
        result.quality_score = 100.0;
        
        // Check for outliers (simple z-score method)
        double mean = analyze("mean");
        double std_dev = analyze("std_dev");
        
        int outlier_count = 0;
        for (double value : data) {
            parser_.set("value", value);
            parser_.set("mean", mean);
            parser_.set("std_dev", std_dev);
            
            double z_score = parser_.eval("abs((value - mean) / std_dev)");
            if (z_score > 3.0) {
                outlier_count++;
            }
        }
        
        if (outlier_count > 0) {
            result.issues.push_back("Found " + std::to_string(outlier_count) + " outliers (|z| > 3)");
            result.quality_score -= outlier_count * 10.0;
        }
        
        // Check coefficient of variation
        double cv = analyze("cv");
        if (cv > 20.0) {
            result.issues.push_back("High variability (CV = " + std::to_string(cv) + "%)");
            result.quality_score -= 20.0;
            result.passed = false;
        }
        
        // Check sample size
        if (data.size() < 10) {
            result.issues.push_back("Small sample size (n = " + std::to_string(data.size()) + ")");
            result.quality_score -= 15.0;
        }
        
        result.quality_score = std::max(0.0, result.quality_score);
        
        return result;
    }
    
    void demonstrate_analysis() {
        std::cout << "=== Scientific Data Analysis System ===" << std::endl;
        
        // Simulated experimental data: reaction rates at different temperatures
        std::vector<double> reaction_rates = {
            2.3, 2.1, 2.4, 2.2, 2.5, 2.0, 2.3, 2.4, 2.1, 2.2,
            2.6, 2.3, 2.1, 2.4, 2.2, 2.5, 2.3, 2.1, 2.4, 2.2
        };
        
        load_dataset(reaction_rates);
        
        // Basic statistical analysis
        std::cout << "Reaction Rate Analysis:" << std::endl;
        std::cout << "Sample size: " << analyze("n") << std::endl;
        std::cout << "Mean: " << analyze("mean") << " mol/L·s" << std::endl;
        std::cout << "Standard deviation: " << analyze("std_dev") << " mol/L·s" << std::endl;
        std::cout << "Coefficient of variation: " << analyze("cv") << "%" << std::endl;
        std::cout << "Standard error: " << analyze("sem") << " mol/L·s" << std::endl;
        
        // Quality control
        auto qc_result = quality_control_check(reaction_rates);
        std::cout << "\nQuality Control:" << std::endl;
        std::cout << "Status: " << (qc_result.passed ? "PASSED" : "FAILED") << std::endl;
        std::cout << "Quality Score: " << qc_result.quality_score << "/100" << std::endl;
        
        if (!qc_result.issues.empty()) {
            std::cout << "Issues:" << std::endl;
            for (const auto& issue : qc_result.issues) {
                std::cout << "  - " << issue << std::endl;
            }
        }
        
        // Chemistry calculations
        std::cout << "\nChemistry Calculations:" << std::endl;
        set_experimental_parameters({
            {"moles", 0.1},
            {"volume_l", 0.5},
            {"h_concentration", 1e-7},
            {"ka", 1.8e-5},
            {"base_conc", 0.1},
            {"acid_conc", 0.05}
        });
        
        std::cout << "Molarity: " << analyze("molarity") << " M" << std::endl;
        std::cout << "pH: " << analyze("ph") << std::endl;
        std::cout << "pKa: " << analyze("pka") << std::endl;
        std::cout << "Henderson-Hasselbalch pH: " << analyze("henderson_hasselbalch") << std::endl;
        
        // Physics calculations
        std::cout << "\nPhysics Calculations:" << std::endl;
        set_experimental_parameters({
            {"mass", 0.001}, // 1 gram
            {"velocity", 10.0}, // 10 m/s
            {"n", 1.0}, // 1 mole
            {"temperature", 298.15}, // 25°C
            {"volume", 0.0224}, // 22.4 L
            {"frequency", 5e14} // 500 THz
        });
        
        std::cout << "Kinetic energy: " << analyze("kinetic_energy") << " J" << std::endl;
        std::cout << "Ideal gas pressure: " << analyze("ideal_gas_pressure") << " Pa" << std::endl;
        std::cout << "Wavelength: " << analyze("wavelength") << " m" << std::endl;
        std::cout << "Photon energy: " << analyze("photon_energy") << " J" << std::endl;
        
        // Error analysis
        std::cout << "\nError Analysis:" << std::endl;
        set_experimental_parameters({
            {"measured", 2.25},
            {"true_value", 2.30},
            {"value", 2.8},
            {"mean", analyze("mean")},
            {"std_dev", analyze("std_dev")}
        });
        
        std::cout << "Relative error: " << analyze("relative_error") << "%" << std::endl;
        std::cout << "Z-score for outlier: " << analyze("z_score") << std::endl;
    }
};

void demonstrate_scientific_data_analysis() {
    ScientificDataProcessor processor;
    processor.demonstrate_analysis();
}
```

## Main Application

```cpp
int main() {
    std::cout << "Modern C++ Calculator - Real-world Usage Examples\n" << std::endl;
    
    try {
        demonstrate_configuration_system();
        std::cout << std::endl;
        
        demonstrate_game_damage_system();
        std::cout << std::endl;
        
        demonstrate_scientific_data_analysis();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "These examples demonstrate production-ready applications:" << std::endl;
    std::cout << "1. Configuration Management - Dynamic system configuration" << std::endl;
    std::cout << "2. Game Development - Flexible damage calculation system" << std::endl;
    std::cout << "3. Scientific Analysis - Laboratory data processing" << std::endl;
    std::cout << "\nKey patterns used:" << std::endl;
    std::cout << "- Caching for performance" << std::endl;
    std::cout << "- Error handling and validation" << std::endl;
    std::cout << "- Modular formula management" << std::endl;
    std::cout << "- Real-time parameter updates" << std::endl;
    std::cout << "- Quality control and monitoring" << std::endl;
    
    return 0;
}
```

## Production Deployment Considerations

### 1. Error Handling and Logging

```cpp
// Comprehensive error handling with logging
class ProductionCalculator {
    // ... implementation with proper logging, monitoring, and error recovery
};
```

### 2. Performance Monitoring

```cpp
// Performance metrics collection
class PerformanceMonitor {
    // ... track evaluation times, cache hit rates, error rates
};
```

### 3. Security Considerations

```cpp
// Input validation and sanitization
class SecureCalculator {
    // ... validate expressions, limit complexity, prevent injection
};
```

### 4. Scalability Patterns

```cpp
// Thread-safe, scalable implementation
class ScalableCalculator {
    // ... thread pools, connection pooling, load balancing
};
```

These real-world examples demonstrate how the calculator library can be integrated into production systems while maintaining performance, reliability, and maintainability.

## Next Steps

- Review [Advanced Examples](advanced-examples.md) for complex mathematical applications
- Check [User Guide](../user-guide/) for comprehensive usage patterns
- See [API Reference](../api-reference/) for complete technical documentation
- Explore [Development Guide](../development/) for contributing and extending the library
