#include "svgPlots.hpp"
#include <sstream>
#include <algorithm>

std::string plot_hrv_svg(const std::vector<DayRecord>& days) {
    const int WIDTH = 600;
    const int HEIGHT = 200;
    const int PADDING = 20;

    std::vector<double> hrv_vals;

    size_t start = days.size() > 28 ? days.size() - 28 : 0;

    for (size_t i = start; i < days.size(); i++) {
        if (days[i].hrv.has_value())
        {
            hrv_vals.push_back(*days[i].hrv);
        }
    }

    if (hrv_vals.empty()) return "<svg width =\"600\" height = \"200\"></svg>";

    double min_hrv = *std::min_element(hrv_vals.begin(), hrv_vals.end());
    double max_hrv = *std::max_element(hrv_vals.begin(), hrv_vals.end());

    std::ostringstream path;
    for (size_t i = 0; i < hrv_vals.size(); i++)
    {
        double x = PADDING + static_cast<double>(i) / (hrv_vals.size() - 1) * (WIDTH - 2 * PADDING);
        double y = HEIGHT - PADDING -
            (hrv_vals[i] - min_hrv) / (max_hrv - min_hrv) * (HEIGHT - 2 * PADDING);

            if (i == 0) {
                path << "m" << x << " " << y << " ";
            } else {
                path << "L" << x << " " << y << " ";
            }
    }

    std::ostringstream svg;
    svg << "<svg width=\"" << WIDTH << "\" height=\"" << HEIGHT
        << "\" xmlns=\"http://www.w3.org/2000/svg\">";
    svg << "<path d=\"" << path.str()
        << "\" stroke=\"blue\" fill=\"none\" stroke-width=\"2\"/>";
    svg << "</svg>";

    return svg.str();
}
