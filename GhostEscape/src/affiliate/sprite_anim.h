#ifndef _SPRITE_ANIM_H_
#define _SPRITE_ANIM_H_

#include "sprite.h"

// 精灵图动画
class SpriteAnim : public Sprite
{
private:
	// 动画帧
	struct Frame
	{
	public:
		SDL_Texture* tex_ = nullptr;
		SDL_FRect src_rect_ = { 0 };
	public:
		Frame(SDL_Texture* tex, const SDL_FRect& src_rect)
			:tex_(tex), src_rect_(src_rect){ }
	};
	typedef std::vector<Frame> FrameList;

protected:
    FrameList frames_;
    int total_frame_count_ = 1;
    int frame_idx_ = 0;
    float frame_interval_ = 0.1f;
    bool is_loop_ = true;
    bool is_finished_ = false;
    float timer_ = 0;   // 计时器

public:
    SpriteAnim(ObjectScreen* parent, ResID tex_id, int frame_count, AchorMode mode = AchorMode::CENTER, 
        const glm::vec2& scale = glm::vec2(1), float frame_interval = 0.1f, float is_loop = true);
    virtual ~SpriteAnim(){};

    virtual void update(float dt);
    virtual void render();

    // setters and getters
    float getFrameInterval() const { return frame_interval_; }
    void setFrameInterval(float interval) { frame_interval_ = interval; }
    void setLoop(bool flag) { is_loop_ = flag; }
    bool getIsFinished() const { return is_finished_; }
    void setFrameIdx(int idx) { frame_idx_ = idx; }
    int getFrameIdx() const { return frame_idx_; }
    void syncFrameTime(SpriteAnim* sprite_anim);
    void restart() { frame_idx_ = 0; timer_ = 0; }
};




#endif // _SPRITE_ANIM_H_