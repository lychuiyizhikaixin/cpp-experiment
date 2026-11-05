#include "Executor.hpp"
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {
struct Model { adas::Pose pose; bool fast=false; bool reverse=false; };
void Move(Model& m, int count, bool reverse)
{
    const int d = reverse ? -count : count;
    switch (m.pose.heading) {
    case 'E': m.pose.x += d; break;
    case 'W': m.pose.x -= d; break;
    case 'N': m.pose.y += d; break;
    case 'S': m.pose.y -= d; break;
    }
}
void Turn(Model& m, bool left)
{
    if (left) {
        switch (m.pose.heading) {
        case 'E': m.pose.heading='N'; break;
        case 'N': m.pose.heading='W'; break;
        case 'W': m.pose.heading='S'; break;
        case 'S': m.pose.heading='E'; break;
        }
    } else {
        switch (m.pose.heading) {
        case 'E': m.pose.heading='S'; break;
        case 'S': m.pose.heading='W'; break;
        case 'W': m.pose.heading='N'; break;
        case 'N': m.pose.heading='E'; break;
        }
    }
}
void Step(Model& m, adas::VehicleType type, const std::string& token)
{
    if (token=="F") { m.fast=!m.fast; return; }
    if (token=="B") { m.reverse=!m.reverse; return; }
    if (token=="TR") {
        if (m.reverse) { return; }
        if (m.fast) { Move(m,1,false); }
        Turn(m,true); Move(m,1,false); Turn(m,true); return;
    }
    if (token=="M") {
        const int normal = type==adas::VehicleType::SportsCar ? 2 : 1;
        Move(m,normal*(m.fast ? 2 : 1),m.reverse); return;
    }
    const bool left = (token=="L") != m.reverse;
    if (type==adas::VehicleType::Bus) { Move(m,m.fast ? 2 : 1,m.reverse); }
    else if (m.fast) { Move(m,1,m.reverse); }
    Turn(m,left);
    if (type==adas::VehicleType::SportsCar) { Move(m,1,m.reverse); }
}
bool Same(const adas::Pose& a,const adas::Pose& b)
{
    return a.x==b.x && a.y==b.y && a.heading==b.heading;
}
std::size_t checks=0;
void Verify(const std::vector<std::string>& tokens)
{
    for (auto type : {adas::VehicleType::Normal,adas::VehicleType::Bus,adas::VehicleType::SportsCar}) {
        for (char heading : {'E','S','W','N'}) {
            Model expected{{2,-3,heading}};
            std::unique_ptr<adas::Executor> split(adas::Executor::NewExecutor(expected.pose,type));
            std::unique_ptr<adas::Executor> whole(adas::Executor::NewExecutor(expected.pose,type));
            if (!split || !whole) { std::cerr << "allocation failure\n"; std::exit(1); }
            std::string input;
            for (const auto& token : tokens) {
                input+=token; Step(expected,type,token); split->Execute(token);
                if (!Same(expected.pose,split->Query())) {
                    std::cerr<<"prefix mismatch type="<<static_cast<int>(type)<<" initial="<<heading<<" input="<<input<<'\n';
                    std::exit(1);
                }
            }
            whole->Execute(input);
            if (!Same(expected.pose,whole->Query())) {
                std::cerr<<"whole input mismatch "<<input<<'\n'; std::exit(1);
            }
            split->Execute("");
            if (!Same(expected.pose,split->Query())) { std::exit(1); }
            ++checks;
        }
    }
}
void Enumerate(std::vector<std::string>& tokens,int remaining)
{
    if (remaining==0) { Verify(tokens); return; }
    for (const std::string token : {"M","L","R","F","B","TR"}) {
        tokens.push_back(token); Enumerate(tokens,remaining-1); tokens.pop_back();
    }
}
}
int main()
{
    std::vector<std::string> tokens;
    for (int length=0;length<=4;++length) { Enumerate(tokens,length); }
    std::cout<<"Reference model PASS: "<<checks<<" cases; 3 vehicles, 4 headings, token length 0..4\n";
}
