#pragma once
#include "../level-effect.h"

#include <vector>
#include "../power-square.h"

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
		virtual void ghostDraw(float alpha) const;

		PowerSquareWatcher(const PowerSquare*);
		PowerSquareWatcher(const PowerSquare*, double count);
		virtual ~PowerSquareWatcher();

	private:
		PowerSquareWatcher() = delete; //make powerupCopy nullptr and check for it in the destructor if this is wanted
		PowerSquareWatcher(const PowerSquareWatcher&) = delete; //honestly not that hard, but I just don't care
	};

protected:
	bool watchAllPowerups;
	std::vector<PowerSquareWatcher*> watching;

public:
	unsigned int getNumWatching() const { return watching.size(); }
	virtual void watchPowerSquare(const PowerSquare*);
	virtual void watchPowerSquare(const PowerSquare*, double timeToRespawn);
	virtual void watchLastPowerSquaresPushed(int count);
	virtual void watchLastPowerSquaresPushed(int count, double timeToRespawn);
	virtual void unwatchPowerSquare(Game_ID powerupID);

public:
	virtual std::string getName() const override { return "respawning_powerups"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void apply() override;
	virtual void tick(const Level* parent) override;
	virtual void doEffects(Level* parent) const override { return; }

	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

	RespawningPowerupsLevelEffect();
	RespawningPowerupsLevelEffect(bool watchEverything);
	virtual ~RespawningPowerupsLevelEffect();
	static LevelEffect* factory(const GenericFactoryConstructionData&);
};
