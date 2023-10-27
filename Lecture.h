#ifndef AED_PROJECT_AULA_H
#define AED_PROJECT_AULA_H
#include <string>
using namespace std;

class Lecture{
    private:
        float duration;
        float startHour;
        string type;
        string weekday;
        string UC_Code;
    public:
        Lecture(const float &duration,const float &startHour, const string &type, const string &weekday, const string &Uc_Code);
        void setDuration(float duration);
        void setStartHour(float startHour);
        const string &getType() const;
        const float &getDuration() const;
        void setType(const string &type);
        const string &getWeekday() const;
        void setWeekday(const string &weekday);
        const string &getUcCode() const;
        bool operator<(const Lecture& other) const;
        const float &getStartHour() const;
};

#endif //AED_PROJECT_AULA_H
