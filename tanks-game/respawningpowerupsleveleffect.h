#pragma once
#include "leveleffect.h"
#include "powersquare.h"
#include "constants.h"
#include <vector>

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class RespawningPowerupsLevelEffect : public LevelEffect {
protected:
	struct PowerSquareWatcher {
	protected:
		Game_ID powerupID;
		double tickCount;
		double maxTickCount;
		bool isGone;
		const PowerSquare* powerupCopy;

	public:
		Game_ID getPowerupID() const { return powerupID; }
		void setIsGone() { isGone = true; }

		virtual void tick();
		virtual void reset(const PowerSquare*);
		virtual void draw() const;

		PowerSquareWatcher(const PowerSquare*);
		PowerSquareWatcher(const PowerSquare*, double count);
		virtual ~PowerSquareWatcher();

	private:
		PowerSquareWatcher() {} //make powerupCopy nullptr and check for it in the destructor if this is wanted
		PowerSquareWatcher(const PowerSquareWatcher&) {} //honestly not that hard, but I just don't care
	};

protected:
	bool watchAllPowerups;
	std::vector<PowerSquareWatcher*> watching;

public:
	int getNumWatching() const { return watching.size(); }
	virtual void watchPowerSquare(const PowerSquare*);
	virtual void unwatchPowerSquare(Game_ID powerupID);

	virtual std::string getName() const override { return "respawning_powerups"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override;
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual void apply() override;
	virtual void tick(const Level* parent) override;
	virtual void doEffects(Level* parent) override;

	virtual void draw() const override;
	//virtual void draw(double xpos, double ypos) const override;
	//virtual void poseDraw() const override;

	RespawningPowerupsLevelEffect();
	RespawningPowerupsLevelEffect(bool watchEverything);
	virtual ~RespawningPowerupsLevelEffect();
	static LevelEffect* factory(int, std::string*);
};
