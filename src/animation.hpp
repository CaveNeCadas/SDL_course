#ifndef ANIMATION__H
#define ANIMATION__H

class Animation
{
    private:
        uint32_t m_index;
        uint32_t m_num_frame;
        uint32_t m_speed;

    public:
        Animation( )
        :Animation(0,0,0)
        {}
        Animation(  uint32_t index,
                    uint32_t num_frame,
                    uint32_t speed 
                )
                : m_index(index)
                , m_num_frame(num_frame)
                , m_speed(speed)
                {/*NOP*/}
        ~Animation()=default;
        uint32_t getIndex() const { return m_index;} 
        uint32_t getNumFrame () const { return m_num_frame;} 
        uint32_t getSpeed() const { return m_speed;} 

        
};



#endif